// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-License-Identifier: BSD-2-Clause

#include "ChooseReferenceWidget.h"

#include <KLocalizedString>
#include <QDebug>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QSettings>
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
      m_verseDisplayBox{ new QTextBrowser }
{
    QSettings settings;
    settings.beginGroup("ChooseReferenceWidget");

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

    switch (static_cast<BibleVersionLoadOption>(settings.value("bibleVersionLoadOption"),
                                                static_cast<int>(BibleVersionLoadOption::Last)))
    {
    case BibleVersionLoadOption::Last:
        m_currentBibleVersion =
            settings.value("lastBibleVersion", m_bibleVersion->currentText()).toString();
        break;
    case BibleVersionLoadOption::Set:
        m_currentBibleVersion =
            settings.value("defaultBibleVersion", m_bibleVersion->currentText()).toString();
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

    // connect the combos
    connect(m_books, SIGNAL(currentIndexChanged(int)), this, SLOT(updateChapterVerseValues()));
    connect(m_chapters, SIGNAL(currentIndexChanged(int)), this, SLOT(updateVerseValues()));
    connect(m_startVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEndVerseValues()));
    connect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEndVerseValues()));

    // these are out here because apparently you can't create variables in a switch
    auto lastBook = settings.value("lastBook").toString();
    auto lastChapter = settings.value("lastChapter").toString();
    auto lastStartVerse = settings.value("lastStartVerse").toString();
    auto lastEndVerse = settings.value("lastEndVerse").toString();

    switch (static_cast<VerseLoadOption>(settings.value("verseLoadOption", 1).toInt()))
    {
    case VerseLoadOption::Last:

        if (lastBook != "" && lastChapter != "" && lastStartVerse != "" && lastEndVerse != "")
        {
            m_books->setCurrentText(lastBook);

            // update the values now to load the data for the chapters and verses
            updateChapterVerseValues();

            m_chapters->setCurrentText(lastChapter);
            m_startVerses->setCurrentText(lastStartVerse);
            m_endVerses->setCurrentText(lastEndVerse);
        }
        else
        {
            updateChapterVerseValues();
            updateSaveVerse();
        }

        break;

    case VerseLoadOption::Random:
        updateChapterVerseValues();
        m_books->setCurrentIndex(QRandomGenerator::global()->generate() % m_books->count());
        m_chapters->setCurrentIndex(QRandomGenerator::global()->generate() % m_chapters->count());
        m_startVerses->setCurrentIndex(QRandomGenerator::global()->generate() %
                                       m_startVerses->count());

        // only display one verse
        m_endVerses->setCurrentIndex(m_startVerses->currentIndex());
        break;

    case VerseLoadOption::Set:
        m_books->setCurrentText(settings.value("defaultBook", "Genesis").toString());
        updateChapterVerseValues();
        m_chapters->setCurrentText(settings.value("defaultChapter", "1").toString());
        m_startVerses->setCurrentText(settings.value("defaultStartVerse", "1").toString());
        m_endVerses->setCurrentText(settings.value("defaultEndVerse", "1").toString());
        break;

    default:
        break;
    }

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

    // ... and make sure that the combo boxes are large enough (try removing this and then
    // viewing the verses for Psalms 119 if you think this is unnecessary!)
    m_chapters->setMinimumContentsLength(3);
    m_startVerses->setMinimumContentsLength(3);
    m_endVerses->setMinimumContentsLength(3);

    connect(m_bibleVersion, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        m_currentBibleVersion = text;
        QSettings settings;
        settings.setValue("ChooseReferenceWidget/lastBibleVersion", m_currentBibleVersion);
        displayVerse();
    });

    auto memorize = new QPushButton{ QIcon::fromTheme("go-next"), i18n("Memorize verse") };
    connect(memorize, &QPushButton::clicked, this, &ChooseReferenceWidget::saveItem);
    connect(memorize, &QPushButton::clicked, this, &ChooseReferenceWidget::runMemorizer);

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
    buttonLayout->addWidget(save);
    buttonLayout->addWidget(memorize);

    layout->addLayout(buttonLayout, 2, 0, 1, 7);

    this->setLayout(layout);

    displayVerse();

    settings.endGroup(); // ChooseReferenceWidget
}

void ChooseReferenceWidget::updateChapterVerseValues()
{
    // prevent lots of signals from being emitted
    disconnect(m_chapters, SIGNAL(currentIndexChanged(int)), this, SLOT(updateVerseValues()));
    disconnect(m_chapters, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSaveVerse()));

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
    updateSaveVerse();

    // reconnect the signals
    connect(m_chapters, SIGNAL(currentIndexChanged(int)), this, SLOT(updateVerseValues()));
    connect(m_chapters, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSaveVerse()));
}

void ChooseReferenceWidget::updateVerseValues()
{
    // prevent lots of signals from being emitted
    disconnect(m_startVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEndVerseValues()));
    disconnect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEndVerseValues()));
    disconnect(m_startVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSaveVerse()));
    disconnect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSaveVerse()));

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

    updateSaveVerse();

    // reconnect the signals
    connect(m_startVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEndVerseValues()));
    connect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEndVerseValues()));
    connect(m_startVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSaveVerse()));
    connect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSaveVerse()));
}

void ChooseReferenceWidget::updateEndVerseValues()
{
    // prevent lots of signals from being emitted
    disconnect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEndVerseValues()));
    disconnect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSaveVerse()));

    if (m_startVerses->currentIndex() > m_endVerses->currentIndex())
        m_endVerses->setCurrentIndex(m_startVerses->currentIndex());

    updateSaveVerse();

    // reconnect the signals
    connect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEndVerseValues()));
    connect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSaveVerse()));
}

void ChooseReferenceWidget::updateSaveVerse()
{
    QSettings settings;

    if (settings.value("ChooseReferenceWidget/saveLastRef", true).toBool())
    {
        settings.setValue("ChooseReferenceWidget/lastBook", m_books->currentText());
        settings.setValue("ChooseReferenceWidget/lastChapter", m_chapters->currentText());
        settings.setValue("ChooseReferenceWidget/lastStartVerse", m_startVerses->currentText());
        settings.setValue("ChooseReferenceWidget/lastEndVerse", m_endVerses->currentText());
    }
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
    QSettings settings;
    settings.beginGroup("savedContent");
    settings.beginGroup("verses");
    settings.setValue(m_books->currentText() + " " + m_chapters->currentText() + ":" +
                          m_startVerses->currentText(),
                      m_books->currentText() + " " + m_chapters->currentText() + ":" +
                          m_endVerses->currentText());
    settings.endGroup();
    settings.endGroup();
}

bool ChooseReferenceWidget::unusable()
{
    return m_unusable;
}
