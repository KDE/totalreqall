// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-License-Identifier: BSD-2-Clause

#include "ChooseReferenceWidget.h"

#include "MainWindow.h"
#include "UserSettings.h"
#include <KLocalizedString>
#include <QDebug>
#include <QFile>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QSettings>
#include <QStandardPaths>
#include <QStringList>
#include <markupfiltmgr.h>
#include <swmgr.h>
#include <swmodule.h>
#include <versekey.h>

ChooseReferenceWidget::ChooseReferenceWidget(QWidget *parent)
    : QWidget(parent),
      m_books{ new QComboBox },
      m_chapters{ new QComboBox },
      m_startVerses{ new QComboBox },
      m_endVerses{ new QComboBox },
      m_bibleVersion{ new QComboBox },
      m_verseDisplayBox{ new QTextBrowser },
      m_speaker{ new QTextToSpeech{ QTextToSpeech::availableEngines().first() } },
      m_speak{ new QPushButton{ QIcon::fromTheme("media-playback-start"), i18n("Speak") } }
{
    auto settings = UserSettings::global();

    // general setup
    setStatusTip(i18n("Choose a verse"));

    // set this up now so that the correct Bible version can be loaded for getting book names
    sword::SWMgr mgr{ new sword::MarkupFilterMgr{ sword::FMT_PLAIN } };

    for (auto item : mgr.getModules())
    {
        auto type = item.second->getType();
        if (strcmp(type, sword::SWMgr::MODTYPE_BIBLES) == 0)
            m_bibleVersion->addItem(item.first.c_str());
    }

    // TODO: this should not show at startup; instead, show it when the user starts this widget
    if (m_bibleVersion->count() == 0) // no Bible modules found
    {
        QMessageBox::critical(this, i18n("No modules found"),
                              i18n("Could not find any Bible modules."));
        m_unusable = true;
    }

    switch (settings->bibleVersionLoadOption())
    {
    case BibleVersionLoadOption::LastVersion:
        m_currentBibleVersion =
            (settings->lastBibleVersion() != "" ? settings->lastBibleVersion() :
                                                  m_bibleVersion->currentText());
        break;
    case BibleVersionLoadOption::RandomVersion:
        m_currentBibleVersion = m_bibleVersion->itemText(QRandomGenerator64::global()->generate() %
                                                         m_bibleVersion->count());
        break;
    case BibleVersionLoadOption::SetVersion:
        m_currentBibleVersion =
            (settings->defaultBibleVersion() != "" ? settings->defaultBibleVersion() :
                                                     m_bibleVersion->currentText());
        break;
    default:
        m_currentBibleVersion = m_bibleVersion->currentText();
        break;
    }

    m_bibleVersion->setCurrentText(m_currentBibleVersion);

    // get the values for the books (chapters and verses will come later)
    sword::SWModule *module = mgr.getModule(m_currentBibleVersion.toStdString().c_str());
    sword::VerseKey *key{ static_cast<sword::VerseKey *>(module->getKey()) };
    QStringList bookList;
    while (!key->popError())
    {
        bookList << key->getBookName();
        key->setBook(key->getBook() + 1);
    }
    m_books->insertItems(0, bookList);

    switch (settings->verseLoadOption())
    {
    case VerseLoadOption::LastVerse:
        if (settings->lastBook() != "" && settings->lastChapter() != "" &&
            settings->lastStartVerse() != "" && settings->lastEndVerse() != "")
        {
            m_books->setCurrentText(settings->lastBook());

            // update the values now to load the data for the chapters and verses
            updateChapterVerseValues();

            m_chapters->setCurrentText(settings->lastChapter());
            m_startVerses->setCurrentText(settings->lastStartVerse());
            m_endVerses->setCurrentText(settings->lastEndVerse());
        }
        else // invalid save data, use whatever is available (normally Genesis 1:1-1)
        {
            updateChapterVerseValues();
            updateSaveVerse();
        }

        break;

    case VerseLoadOption::RandomVerse:
        updateChapterVerseValues();
        m_books->setCurrentIndex(QRandomGenerator::global()->generate() % m_books->count());
        m_chapters->setCurrentIndex(QRandomGenerator::global()->generate() % m_chapters->count());
        m_startVerses->setCurrentIndex(QRandomGenerator::global()->generate() %
                                       m_startVerses->count());

        // only display one verse
        m_endVerses->setCurrentIndex(m_startVerses->currentIndex());
        break;

    case VerseLoadOption::SetVerse:
        if (settings->defaultBook() != "" && settings->defaultChapter() != "" &&
            settings->defaultStartVerse() != "" && settings->defaultEndVerse() != "")
        {
            m_books->setCurrentText(settings->defaultBook());
            updateChapterVerseValues();
            m_chapters->setCurrentText(settings->defaultChapter());
            m_startVerses->setCurrentText(settings->defaultStartVerse());
            m_endVerses->setCurrentText(settings->defaultEndVerse());
        }
        else // invalid save data, use whatever is available (normally Genesis 1:1-1)
        {
            updateChapterVerseValues();
            updateSaveVerse();
        }
        break;

    default:
        updateChapterVerseValues();
        updateSaveVerse();
        break;
    }

    // make sure that the combo boxes are large enough (try removing this and then
    // viewing the verses for Psalms 119 if you think this is unnecessary!)
    m_chapters->setMinimumContentsLength(3);
    m_startVerses->setMinimumContentsLength(3);
    m_endVerses->setMinimumContentsLength(3);

    auto memorize = new QPushButton{ QIcon::fromTheme("go-next"), i18n("Memorize verse") };
    connect(memorize, &QPushButton::clicked, this, &ChooseReferenceWidget::saveItem);
    connect(memorize, &QPushButton::clicked, this, &ChooseReferenceWidget::runMemorizer);

    connect(m_speak, &QPushButton::clicked, this, [this]() {
        if (m_speaker->state() == QTextToSpeech::Ready)
        {
            QString reference{ "%1 %2:%3" };
            int extraVerses = (m_endVerses->currentIndex() > m_startVerses->currentIndex()) ?
                                  (m_endVerses->currentIndex() - m_startVerses->currentIndex()) :
                                  0;

            sword::SWMgr mgr{ new sword::MarkupFilterMgr{ sword::FMT_PLAIN } };
            sword::SWModule *module = mgr.getModule(m_currentBibleVersion.toStdString().c_str());
            QString content;

            sword::SWKey key{ reference
                                  .arg(m_books->currentText(), m_chapters->currentText(),
                                       m_startVerses->currentText())
                                  .toStdString()
                                  .c_str() };
            module->setKey(key);

            for (int i = 0; i <= extraVerses; ++i)
            {
                content.append(module->renderText() + " ");
                module->increment();
            }
            m_speaker->say(content);
        }
        else if (m_speaker->state() == QTextToSpeech::Speaking)
            m_speaker->stop();
    });

    connect(m_speaker, &QTextToSpeech::stateChanged, this, [this](QTextToSpeech::State s) {
        if (s == QTextToSpeech::Ready)
            m_speak->setIcon(QIcon::fromTheme("media-playback-start"));
        else if (s == QTextToSpeech::Speaking)
            m_speak->setIcon(QIcon::fromTheme("media-playback-stop"));
    });

    auto save = new QPushButton{ QIcon::fromTheme("document-save"), i18n("Save") };
    connect(save, &QPushButton::clicked, this, &ChooseReferenceWidget::saveItem);

    auto back = new QPushButton{ QIcon::fromTheme("go-previous"), i18n("Back") };
    connect(back, &QPushButton::clicked, this, &ChooseReferenceWidget::cancel);

    m_verseDisplayBox->setFocusPolicy(Qt::ClickFocus);

    auto colon = new QLabel{ ":" };
    colon->setAlignment(Qt::AlignHCenter);
    auto dash = new QLabel{ "-" };
    dash->setAlignment(Qt::AlignHCenter);

    // set up the layout
    auto layout = new QGridLayout;

    layout->addWidget(m_books, 0, 0);
    layout->addWidget(m_chapters, 0, 1);
    layout->addWidget(colon, 0, 2);
    layout->addWidget(m_startVerses, 0, 3);
    layout->addWidget(dash, 0, 4);
    layout->addWidget(m_endVerses, 0, 5);
    layout->addWidget(m_bibleVersion, 0, 6);

    layout->addWidget(m_verseDisplayBox, 1, 0, 1, 7);

    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(back);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_speak);
    buttonLayout->addWidget(save);
    buttonLayout->addWidget(memorize);

    layout->addLayout(buttonLayout, 2, 0, 1, 7);

    this->setLayout(layout);

    connect(m_bibleVersion, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        m_currentBibleVersion = text;
        UserSettings::global()->setLastBibleVersion(m_currentBibleVersion);
        displayVerse();
    });

    // connect the combos
    connect(m_books, SIGNAL(currentIndexChanged(int)), this, SLOT(updateChapterVerseValues()));
    connect(m_chapters, SIGNAL(currentIndexChanged(int)), this, SLOT(updateVerseValues()));
    connect(m_startVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEndVerseValues()));
    connect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEndVerseValues()));

    // we also need to save all info about the last verse
    connect(m_books, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSaveVerse()));
    connect(m_chapters, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSaveVerse()));
    connect(m_startVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSaveVerse()));
    connect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSaveVerse()));

    // and we want to always display the currently selected verse
    connect(m_books, SIGNAL(currentIndexChanged(int)), this, SLOT(displayVerse()));
    connect(m_chapters, SIGNAL(currentIndexChanged(int)), this, SLOT(displayVerse()));
    connect(m_startVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(displayVerse()));
    connect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(displayVerse()));

    updateChapterVerseValues();
    updateSaveVerse();

    displayVerse();
}

void ChooseReferenceWidget::updateChapterVerseValues()
{
    // prevent lots of signals from being emitted
    disconnect(m_chapters, SIGNAL(currentIndexChanged(int)), this, SLOT(updateVerseValues()));

    // set up sword
    sword::SWMgr mgr{ new sword::MarkupFilterMgr{ sword::FMT_PLAIN } };
    sword::SWModule *module = mgr.getModule(m_currentBibleVersion.toStdString().c_str());
    sword::VerseKey *key{ static_cast<sword::VerseKey *>(module->getKey()) };
    key->setBookName(m_books->currentText().toStdString().c_str());

    // first take care of the chapter
    // get data to insert
    int chapters = key->getChapterMax();

    // make sure that we do not set the current index to -1, use the first item instead
    auto old = (m_chapters->currentIndex() == -1) ? 0 : m_chapters->currentIndex();

    // remove existing items
    m_chapters->clear();

    // insert the new data
    QStringList chapList;

    for (int i = 0; i < chapters; ++i)
    {
        QString temp{ "%1" };
        temp = temp.arg(i + 1);
        chapList.push_back(temp); // + 1 to ensure that we insert at the end
    }

    m_chapters->insertItems(0, chapList);

    // this triggers a call to updateVerseValues()
    if ((m_chapters->count() - 1) >= old)
        m_chapters->setCurrentIndex(old);
    else // use the closest thing to the old index that we have
        m_chapters->setCurrentIndex(m_chapters->count() - 1);

    updateVerseValues();

    // reconnect the signal
    connect(m_chapters, SIGNAL(currentIndexChanged(int)), this, SLOT(updateVerseValues()));
}

void ChooseReferenceWidget::updateVerseValues()
{
    // prevent lots of signals from being emitted
    disconnect(m_startVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEndVerseValues()));
    disconnect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEndVerseValues()));

    // set up sword
    sword::SWMgr mgr{ new sword::MarkupFilterMgr{ sword::FMT_PLAIN } };
    sword::SWModule *module = mgr.getModule(m_currentBibleVersion.toStdString().c_str());
    sword::VerseKey *key{ static_cast<sword::VerseKey *>(module->getKey()) };
    key->setBookName(m_books->currentText().toStdString().c_str());
    key->setChapter(m_chapters->currentText().toInt());

    // get data to insert
    int verses = key->getVerseMax();

    // make sure that we do not set the current index to -1, use the first item instead
    auto oldStart = (m_startVerses->currentIndex() == -1) ? 0 : m_startVerses->currentIndex();
    auto oldEnd = (m_endVerses->currentIndex() == -1) ? 0 : m_endVerses->currentIndex();

    // remove existing items
    m_startVerses->clear();
    m_endVerses->clear();

    // insert the new data
    QStringList verseList;

    for (int i = 0; i < verses; ++i)
    {
        QString temp{ "%1" };
        temp = temp.arg(i + 1);
        verseList.push_back(temp);
    }

    m_startVerses->insertItems(0, verseList);
    m_endVerses->insertItems(0, verseList);

    // make sure that the old index is still valid
    // - 1 because count isn't zero numbered but the index is
    if ((m_startVerses->count() - 1) >= oldStart)
        m_startVerses->setCurrentIndex(oldStart);
    else // use the closest thing to the old index that we have
        m_startVerses->setCurrentIndex(m_startVerses->count() - 1);

    if ((m_endVerses->count() - 1) >= oldEnd)
        m_endVerses->setCurrentIndex(oldEnd);
    else
        m_endVerses->setCurrentIndex(m_endVerses->count() - 1);

    // make sure that m_endVerses has a proper index
    updateEndVerseValues();

    // reconnect the signals
    connect(m_startVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEndVerseValues()));
    connect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEndVerseValues()));
}

void ChooseReferenceWidget::updateEndVerseValues()
{
    // prevent lots of signals from being emitted
    disconnect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEndVerseValues()));

    if (m_startVerses->currentIndex() > m_endVerses->currentIndex())
        m_endVerses->setCurrentIndex(m_startVerses->currentIndex());

    // reconnect the signal
    connect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEndVerseValues()));
}

void ChooseReferenceWidget::updateSaveVerse()
{
    UserSettings::global()->setLastBook(m_books->currentText());
    UserSettings::global()->setLastChapter(m_chapters->currentText());
    UserSettings::global()->setLastStartVerse(m_startVerses->currentText());
    UserSettings::global()->setLastEndVerse(m_endVerses->currentText());
}

void ChooseReferenceWidget::runMemorizer()
{
    QString reference{ "%1 %2:%3" };
    int extraVerses = (m_endVerses->currentIndex() > m_startVerses->currentIndex()) ?
                          (m_endVerses->currentIndex() - m_startVerses->currentIndex()) :
                          0;

    sword::SWMgr mgr{ new sword::MarkupFilterMgr{ sword::FMT_PLAIN } };
    sword::SWModule *module = mgr.getModule(m_currentBibleVersion.toStdString().c_str());
    QString content;

    sword::SWKey key{ reference
                          .arg(m_books->currentText(), m_chapters->currentText(),
                               m_startVerses->currentText())
                          .toStdString()
                          .c_str() };
    module->setKey(key);

    for (int i = 0; i <= extraVerses; ++i)
    {
        content.append(module->renderText() + "\n");
        module->increment();
    }

    emit startMemorizer(content);
}

void ChooseReferenceWidget::displayVerse()
{
    QString reference{ "%1 %2:%3" };
    int extraVerses = (m_endVerses->currentIndex() > m_startVerses->currentIndex()) ?
                          (m_endVerses->currentIndex() - m_startVerses->currentIndex()) :
                          0;

    sword::SWMgr mgr{ new sword::MarkupFilterMgr{ sword::FMT_PLAIN } };
    sword::SWModule *module = mgr.getModule(m_currentBibleVersion.toStdString().c_str());
    QString content;

    sword::SWKey key{ reference
                          .arg(m_books->currentText(), m_chapters->currentText(),
                               m_startVerses->currentText())
                          .toStdString()
                          .c_str() };
    module->setKey(key);

    for (int i = 0; i <= extraVerses; ++i)
    {
        content.append(module->renderText() + "<br>");
        module->increment();
    }

    m_verseDisplayBox->setHtml(content);
}

void ChooseReferenceWidget::saveItem()
{
    QFile file{ QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/totalreqall-"
                                                                                    "saves.json" };
    if (file.open(QIODevice::ReadWrite))
    {
        QJsonObject item;
        item.insert("type", "verse");
        item.insert("title", m_books->currentText() + " " + m_chapters->currentText() + ":" +
                                 m_startVerses->currentText() + "-" + m_endVerses->currentText());
        item.insert("startRef", m_books->currentText() + " " + m_chapters->currentText() + ":" +
                                    m_startVerses->currentText());
        item.insert("endRef", m_books->currentText() + " " + m_chapters->currentText() + ":" +
                                  m_endVerses->currentText());
        item.insert("numExtraVerses",
                    m_endVerses->currentText().toInt() - m_startVerses->currentText().toInt());
        item.insert("bibleVersion", m_bibleVersion->currentText());
        item.insert("completed", false);

        QJsonDocument doc{ QJsonDocument::fromJson(file.readAll()) };
        QJsonArray a;

        if (doc.isArray())
            a = doc.array();

        a.append(item);
        doc.setArray(a);
        file.resize(0);
        QTextStream s(&file);
        s << doc.toJson();
        file.close();
    }
    else
        qDebug() << "File couldn't be opened";
}

bool ChooseReferenceWidget::unusable()
{
    return m_unusable;
}
