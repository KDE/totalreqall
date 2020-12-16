// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com
// SPDX-License-Identifier: BSD-2-Clause

#include "MemorizeWidget.h"

#include "UserSettings.h"
#include <KLocalizedString>
#include <QDebug>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QRandomGenerator>
#include <QStringList>
#include <map>
#include <string>

QSet<QString> MemorizeWidget::s_memorizedContent = QSet<QString>();

MemorizeEdit::MemorizeEdit(QString &memorizeContent, Difficulty difficulty, QWidget *parent)
    : QTextEdit{ parent },
      m_difficulty{ difficulty }
{
    auto settings = UserSettings::global();

    // the static_cast is necessary here
    m_errorAction = settings->memorizeErrorAction();

    setAcceptRichText(true);
    setTabChangesFocus(true);
    setReadOnly(true);

    // add a space after each newline so that words break appropriately at newlines (e.g. "word1\n"
    // "word2" instead of "word1\nword2") and also change the "\n" to a "<br>" to play right with
    // the HTML text std::string is the easiest way to do this
    auto temp = memorizeContent.toStdString();

    for (std::string::size_type pos = temp.find("\n", 0); pos != std::string::npos;
         pos = temp.find("\n", pos))
        temp.replace(pos, 1, "<br> ");

    QString newMemContent{ temp.c_str() };
    m_words = newMemContent.split(" ", QString::SplitBehavior::SkipEmptyParts);

    m_numWords = m_words.size();
    setHtml(formattedEndString(m_difficulty));
}

void MemorizeEdit::keyPressEvent(QKeyEvent *event)
{
    // here we take care of keys that are expected to do something (like Ctrl-A: select all)
    if (event->key() == Qt::Key::Key_A && event->modifiers() == Qt::ControlModifier)
        this->selectAll();

    // provide a hint
    else if (event->key() == Qt::Key::Key_Question)
    {
        // Make sure that we don't put a space after a newline.
        m_richText += "<span style=\"color:red;\">" + m_words[0];

        if (m_richText.at(m_richText.length() - 1) != "\n")
            m_richText += " ";

        m_richText += "</span>";

        // Delete this word now so that calculations don't get messed up in formattedEndString()
        m_words.pop_front();

        setHtml(m_richText + formattedEndString(m_difficulty));

        emit messageToUser("Hint provided.");
    }

    // skip all keys but the letters and numbers
    else if (!((event->key() >= 0x30 && event->key() <= 0x39) ||
               (event->key() >= 0x41 && event->key() <= 0x5a)))
        return;

    // make sure there's still something to take care of
    else if (m_words.length() > 0)
    {
        // Get the first letter of the word. This is designed to prevent one from
        // having to input punctuation (e.g. for the string "...but" type "b"
        // instead of ".")
        QChar firstChar;

        for (int i = 0; i < m_words[0].length(); ++i)
        {
            if (m_words[0][i].isLetterOrNumber())
            {
                firstChar = m_words[0][i];
                break;
            }
        }

        // Was the correct letter typed?
        if (event->key() == firstChar.toUpper().unicode())
        {
            m_richText += m_words[0];

            if (m_richText.at(m_richText.length() - 1) != "\n")
                m_richText += " ";

            // Delete this word now so that calculations don't get messed up in formattedEndString()
            m_words.pop_front();

            setHtml(m_richText + formattedEndString(m_difficulty));

            emit messageToUser("");
        }
        else
        {
            switch (m_errorAction)
            {
            case ErrorAction::Redo:
                emit messageToUser(i18n("Oops, you messed up! Try again."));
                break;

            case ErrorAction::KeepGoing:
                m_richText += "<span style=\"color:red;\">" + m_words[0];

                if (m_richText.at(m_richText.length() - 1) != "\n")
                    m_richText += " ";

                m_richText += "</span>";
                m_words.pop_front();
                setHtml(m_richText + formattedEndString(m_difficulty));

                emit messageToUser(i18n("Oops, you messed up!"));
                break;

            default:
                break;
            }
        }
    }

    // we're done with the memorization, clean up and shut down
    if (m_words.length() < 1)
    {
        // remove the "\n" at the end so that the box doesn't scroll down and hide some text
        if (m_richText.endsWith("<br> "))
            this->setHtml(m_richText.remove(m_richText.length() - 5, 5));
        else if (m_richText.endsWith("<br> </span>"))
            this->setHtml(m_richText.remove(m_richText.length() - 12, 12));

        emit messageToUser(i18n("Done!"));
        emit done();
    }
}

QString MemorizeEdit::formattedEndString(Difficulty difficulty)
{
    QString formatted;
    int numWordsUsed = 0;
    // static because we don't want to switch which words are displayed halfway through memorization
    static int mediumDifficultyOffset = QRandomGenerator::global()->generate() % 2;

    switch (difficulty)
    {
    case Difficulty::Easy:
        formatted = "<span style=\"color:#8b8b8b;\">" + m_words.join(" ") + "</span>";
        break;

    case Difficulty::Medium:
        numWordsUsed = m_numWords - m_words.length();

        for (int i = numWordsUsed; i < m_numWords; ++i)
        {
            if (i % 2 == mediumDifficultyOffset)
                formatted +=
                    "<span style=\"color:#8b8b8b;\">" + m_words[i - numWordsUsed] + " </span>";
            else
                formatted +=
                    "<span style=\"color:#00000000;\">" + m_words[i - numWordsUsed] + " </span>";
        }
        break;

    case Difficulty::Hard:
        formatted = "";
        break;

    default:
        break;
    }
    return formatted;
}

MemorizeWidget::MemorizeWidget(QString memorizeContent, QWidget *parent)
    : QWidget{ parent },
      m_layout{ new QGridLayout{ this } },
      m_endSession{ new QPushButton },
      m_endMemorizerTimer{ new QTimer }
{
    // clear the status bar
    emit newStatus("");

    if (UserSettings::global()->splitContent())
    {
        if (memorizeContent.split("\n\n").size() > 1)
        {
            m_content = memorizeContent.split("\n\n");
            m_sep = "\n\n";
        }
        else if (memorizeContent.split("\n").size() > 2)
        {
            m_content = memorizeContent.split("\n");
            m_sep = "\n";
        }
        else
            m_content << memorizeContent;
    }
    else
        m_content << memorizeContent;

    if (!MemorizeWidget::s_memorizedContent.contains(m_content.join(m_sep)))
        m_difficulty = Difficulty::Easy; // completely memorize this
    else
        m_difficulty = Difficulty::Hard; // we know this already

    // set up the widgets...
    m_memorizeEdit = new MemorizeEdit{ m_content[m_contentIndex], m_difficulty };
    m_endSession->setText(i18n("Stop memorizing"));

    connect(m_memorizeEdit, &MemorizeEdit::done, this, &MemorizeWidget::editDone);
    connect(m_endSession, &QPushButton::clicked, this, &MemorizeWidget::done);
    connect(m_memorizeEdit, &MemorizeEdit::messageToUser, this, &MemorizeWidget::newStatus);

    // ...and add them to the layout
    auto btnRow = new QHBoxLayout;
    btnRow->insertStretch(0);
    btnRow->insertWidget(1, m_endSession);
    m_layout->addWidget(m_memorizeEdit, 0, 0);
    m_layout->addLayout(btnRow, 1, 0);

    // now we can set the layout
    this->setLayout(m_layout);

    // grab the focus
    m_memorizeEdit->setFocus();
}

MemorizeWidget::~MemorizeWidget() {}

void MemorizeWidget::editDone()
{
    // remove this connection (now we want to go to the next level)
    disconnect(m_endSession, &QPushButton::clicked, this, &MemorizeWidget::done);
    connect(m_endSession, &QPushButton::clicked, this, &MemorizeWidget::nextLevel);
    m_endSession->setText(i18n("Continue"));
    m_endSession->setFocus();
}

void MemorizeWidget::nextLevel()
{
    // set up the new level
    if (m_difficulty < Difficulty::Hard)
        m_difficulty = static_cast<Difficulty>(m_difficulty + 1);

    // we've fully memorized this, so it's time to either move to the next chunk of data or end the loop6
    else if (m_content.size() == 1)
    {
        // this is a special case, because using normal handling on content size 1 will result in running the whole memorization process 3 times (in my expericence)
        // register this as having been memorized in this session
        s_memorizedContent.insert(m_content.first());

        m_endSession->setText(i18n("Continue"));
        emit done();
        return; // skip the memorize process
    }
    else if (m_contentIndex < m_content.size())
    {
        ++m_contentIndex;
        m_difficulty = Difficulty::Easy;
    }
    else if (m_contentIndex == m_content.size())
    {
        ++m_contentIndex;
        m_difficulty = Difficulty::Easy;
    }
    else
    {
        // register this as having been memorized in this session
        s_memorizedContent.insert(m_content.join(" "));

        m_endSession->setText(i18n("Continue"));
        emit done();
        return; // skip the memorize process
    }

    // grab the appropriate content
    QString content = (m_contentIndex < m_content.size()) ? m_content[m_contentIndex] : m_content.join(m_sep);

    m_endSession->setText(i18n("Stop memorizing"));
    disconnect(m_endSession, &QPushButton::clicked, this, &MemorizeWidget::nextLevel);
    connect(m_endSession, &QPushButton::clicked, this, &MemorizeWidget::done);

    delete m_memorizeEdit;
    m_memorizeEdit = new MemorizeEdit{ content, m_difficulty };
    connect(m_memorizeEdit, &MemorizeEdit::done, this, &MemorizeWidget::editDone);
    m_layout->addWidget(m_memorizeEdit, 0, 0);
    m_memorizeEdit->setFocus();
}
