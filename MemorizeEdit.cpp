#include "MemorizeEdit.h"

#include <string>

MemorizeEdit::MemorizeEdit(QString &memorizeContent, QWidget *parent)
    : QTextEdit{ parent },
      m_isDone{ false }
{
	setAcceptRichText(false);

	auto temp = memorizeContent.toStdString();
	for (std::string::size_type pos = temp.find("\n", 0); pos != std::string::npos; pos = temp.find("\n", pos))
	{
		temp.replace(pos, 1, "\n ");
		pos += 2; // get past the \n we just inserted
	}

	QString newMemContent{ temp.c_str() };
	m_words = newMemContent.split(" ");

	// remove the empty string that may be at the end of the list
	if (m_words.last() == QString{ "" })
		m_words.pop_back();
}

void MemorizeEdit::keyPressEvent(QKeyEvent *event)
{
	// here we take care of keys that are expected to do something (like Ctrl-A: select all)
	if (event->key() == Qt::Key::Key_A && event->modifiers() == Qt::ControlModifier)
		this->selectAll();

	// TODO: handle arrow keys properly

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
			setText(this->toPlainText() + " " + m_words[0]);
			moveCursor(QTextCursor::MoveOperation::End);

			// Delete this word.
			m_words.pop_front();

			emit messageToUser("");
		}
		else
			emit messageToUser(tr("Oops, you messed up! Try again."));
	}

	// we're done with the memorization, clean up and shut down
	// m_isDone is only designed to prevent the timer from being delayed by extra keypresses
	if (m_words.length() < 1 && !m_isDone)
	{
		clearFocus();
		emit messageToUser(tr("Done!"));
		emit done();
		m_isDone = true;
	}
}
