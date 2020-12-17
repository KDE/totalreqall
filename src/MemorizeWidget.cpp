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

    // Add a space after each newline so that words break appropriately at newlines (e.g. "word1\n"
    // "word2" instead of "word1\nword2") and also change the "\n" to a "<br>" to play right with
    // the HTML text. std::string is the easiest way to do this
    auto temp = memorizeContent.toStdString();

    // delete all the extra newlines found
//    for (std::string::size_type pos = temp.find("\n\n", 0); pos != std::string::npos;
//         pos = temp.find("\n\n", pos))
//        temp.replace(pos, 2, "\n");

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
    }

    // skip all keys but the letters and numbers
    else if (!((event->key() >= 0x30 && event->key() <= 0x39) ||
               (event->key() >= 0x41 && event->key() <= 0x5a)))
        return;

    // make sure there's still something to take care of
    else if (m_words.length() > 0)
    {
        // Some words aren't going to be things that we want to actually memorize, so let's take care of them.
        // This should likely be handled somewhere else, though
        if (m_words[0].isNull() || m_words[0] == "" || m_words[0] == "<br>")
        {
            m_richText.append(m_words.front());
            m_words.pop_front();
        }

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
        }
        else
        {
            switch (m_errorAction)
            {
            case ErrorAction::Redo:
                break;

            case ErrorAction::KeepGoing:
                m_richText += "<span style=\"color:red;\">" + m_words[0];

                if (m_richText.at(m_richText.length() - 1) != "\n")
                    m_richText += " ";

                m_richText += "</span>";
                m_words.pop_front();
                setHtml(m_richText + formattedEndString(m_difficulty));
                break;

            default:
                break;
            }
        }
    }

    setReadOnly(false);
    textCursor().setPosition(document()->toPlainText().length());
    textCursor().movePosition(QTextCursor::End);
    for (auto x : m_words)
        textCursor().movePosition(QTextCursor::WordLeft);
    ensureCursorVisible();
    setReadOnly(true);

    // we're done with the memorization, clean up and shut down
    if (m_words.length() < 1)
    {
        // remove the "\n" at the end so that the box doesn't scroll down and hide some text
        if (m_richText.endsWith("<br> "))
            this->setHtml(m_richText.remove(m_richText.length() - 5, 5));
        else if (m_richText.endsWith("<br> </span>"))
            this->setHtml(m_richText.remove(m_richText.length() - 12, 12));

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
      m_originalContent{ memorizeContent }
{
    auto settings = UserSettings::global();

    m_content << memorizeContent;
    if (settings->splitContent())
    {
        bool allSegmentsProperSize = false;
        do
        {
            for (int i = 0; i < m_content.size(); ++i)
            {
                if (m_content.at(i).size() > settings->splitThreshold())
                {
                    if (m_content[i].split("\n\n").size() > 1)
                    {
                        QString sep = "\n\n";
                        QStringList split = m_content.at(i).split(sep);
                        m_content.removeAt(i);
                        for (int j = 0; j < split.size(); ++j)
                        {
                            if (split[j].size() < settings->chunkSize() && QString(split[j] + split[j + 1]).size() < settings->chunkSize())
                            {
                                m_content.insert(i + j, split[j] + sep + split[j + 1]);
                                split.removeAt(j + 1);
                            }
                            else
                                m_content.insert(i + j, split[j]);
                        }
                        continue;
                    }

                    // This is not an "else if" because the previous "if" may have split m_content[i] into a chunk that is still too large,
                    // meaning that this block needs to run as well
                    if (m_content[i].split("\n").size() > 2)
                    {
                        QString sep = "\n";
                        QStringList split = m_content.at(i).split(sep);
                        m_content.removeAt(i);
                        for (int j = 0; j < split.size(); ++j)
                        {
                            if (split[j].size() < settings->chunkSize() && QString(split[j] + split[j + 1]).size() < settings->chunkSize())
                            {
                                m_content.insert(i + j, split[j] + sep + split[j + 1]);
                                split.removeAt(j + 1);
                            }
                            else
                                m_content.insert(i + j, split[j]);
                        }
                        continue;
                    }
                    else
                        allSegmentsProperSize = true; // this couldn't be split
                }
            }
            allSegmentsProperSize = true;
            for (auto i : m_content)
                allSegmentsProperSize &= (i.size() < settings->splitThreshold());
        } while (!allSegmentsProperSize);
    }
    else
        m_content << memorizeContent;

    if (!MemorizeWidget::s_memorizedContent.contains(m_originalContent))
        m_difficulty = Difficulty::Easy; // completely memorize this
    else
        m_difficulty = Difficulty::Hard; // we know this already

    // set up the widgets...
    m_memorizeEdit = new MemorizeEdit{ m_content.first(), m_difficulty };
    m_endSession->setText(i18n("Stop memorizing"));

    connect(m_memorizeEdit, &MemorizeEdit::done, this, &MemorizeWidget::editDone);
    connect(m_endSession, &QPushButton::clicked, this, &MemorizeWidget::done);

    // ...and add them to the layout
    auto btnRow = new QHBoxLayout;
    btnRow->insertStretch(0);
    btnRow->insertWidget(1, m_endSession);
    m_layout->addWidget(m_memorizeEdit, 0, 0);
    m_layout->addLayout(btnRow, 1, 0);

    m_memorizeEdit->setFocus();

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

    // we've fully memorized this chunk of data, so it's time to either move to the next chunk of
    // data or end the loop
    else if (m_content.size() == 1)
    {
        // this is a special case, because using normal handling on content size 1 will result in
        // running the whole memorization process 3 times (in my expericence)

        // register this as having been memorized in this session
        s_memorizedContent.insert(m_originalContent);

        m_endSession->setText(i18n("Continue"));
        emit done();
        return; // skip the memorize process
    }
    else if (m_contentIndex < m_content.size())
    {
        // there are still some chunks of data left to memorize
        ++m_contentIndex;
        m_difficulty = Difficulty::Easy;
    }
    else if (m_contentIndex == m_content.size())
    {
        // we've memorized all the data chunks, increment the index to clearly state that there
        // aren't any more chunks to process
        ++m_contentIndex;
        m_difficulty = Difficulty::Easy;
    }
    else
    {
        // register this as having been memorized in this session
        s_memorizedContent.insert(m_originalContent);

        m_endSession->setText(i18n("Continue"));
        emit done();
        return; // skip the memorize process
    }

    // grab the appropriate content
    QString content =
        (m_contentIndex < m_content.size()) ? m_content[m_contentIndex] : m_originalContent;

    m_endSession->setText(i18n("Stop memorizing"));
    disconnect(m_endSession, &QPushButton::clicked, this, &MemorizeWidget::nextLevel);
    connect(m_endSession, &QPushButton::clicked, this, &MemorizeWidget::done);

    delete m_memorizeEdit;
    m_memorizeEdit = new MemorizeEdit{ content, m_difficulty };
    connect(m_memorizeEdit, &MemorizeEdit::done, this, &MemorizeWidget::editDone);
    m_layout->addWidget(m_memorizeEdit, 0, 0);
    m_memorizeEdit->setFocus();
}
