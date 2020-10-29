#include "MemorizeWidget.h"

#include <map>

#include <QGridLayout>
#include <QDebug>
#include <QStringList>

MemorizeWidget::MemorizeWidget(QString memorizeContent, QWidget *parent)
    : QWidget{ parent },
      m_displayText{ new QLabel },
      m_statusLabel{ new QLabel },
      m_layout{ new QGridLayout{ this } }
{
    // set up the widgets...
    m_statusLabel->setText("");
	m_displayText->setWordWrap(true);

    // ...and add them to the layout
    m_layout->addWidget(new QLabel{ tr("Type the first letter of each word.") }, 0, 0);
    m_layout->addWidget(m_displayText, 1, 0);
    m_layout->addWidget(m_statusLabel, 2, 0);

    // now we can set the layout
    this->setLayout(m_layout);

    // go ahead and break up the string
    m_words = memorizeContent.split(" ");

    // get all the input
    grabKeyboard();
}

MemorizeWidget::~MemorizeWidget()
{
}

void MemorizeWidget::keyPressEvent(QKeyEvent *event)
{
    // make sure there's still something to take care of
    if (m_words.length() > 0)
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
            m_displayText->setText(m_displayText->text() + " " + m_words[0]);

            // Delete this word.
            m_words.pop_front();

            // No errors to log.
            m_statusLabel->setText("");
        }
        else
            m_statusLabel->setText(tr("Oops, you messed up! Try again."));
    }
    if (m_words.length() < 1) // we're done with the memorization, clean up and shut down
    {
		releaseKeyboard();
		m_statusLabel->setText(tr("Done!"));

		// Destroy after 1 sec (give the user time to see the verse they just completed
		m_endMemorizerTimer = new QTimer;
		m_endMemorizerTimer->setInterval(1000);
		m_endMemorizerTimer->setSingleShot(true);
		connect(m_endMemorizerTimer, &QTimer::timeout, this, &MemorizeWidget::done);
		m_endMemorizerTimer->start();
    }
}
