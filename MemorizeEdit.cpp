#include "MemorizeEdit.h"

#include <string>

MemorizeEdit::MemorizeEdit(QString &memorizeContent, QWidget *parent)
    : QTextEdit{ parent }
{
	setAcceptRichText(false);

	// add a space after each newline so that words break appropriately at newlines (e.g. "word1\n" "word2"
	// instead of "word1\nword2")
	auto temp = memorizeContent.toStdString();
	for (std::string::size_type pos = temp.find("\n", 0); pos != std::string::npos; pos = temp.find("\n", pos))
	{
		temp.replace(pos, 1, "\n ");
		pos += 2; // get past the \n we just inserted
	}

	QString newMemContent{ temp.c_str() };
	m_words = newMemContent.split(" ", QString::SplitBehavior::SkipEmptyParts);
}

void MemorizeEdit::keyPressEvent(QKeyEvent *event)
{
	// here we take care of keys that are expected to do something (like Ctrl-A: select all)
	if (event->key() == Qt::Key::Key_A && event->modifiers() == Qt::ControlModifier)
		this->selectAll();

	// provide a hint
	else if (event->key() == Qt::Key::Key_Question)
	{
		setText(this->toPlainText() + " " + m_words[0]);
		moveCursor(QTextCursor::MoveOperation::End);

		// Delete this word.
		m_words.pop_front();
		emit messageToUser("Hint provided.");
	}

	// handle arrow keys properly
	else if (event->key() == Qt::Key::Key_Left)
	{
		moveCursor(QTextCursor::MoveOperation::Left);
	}
	else if (event->key() == Qt::Key::Key_Right)
	{
		moveCursor(QTextCursor::MoveOperation::Right);
	}
	else if (event->key() == Qt::Key::Key_Up)
	{
		moveCursor(QTextCursor::MoveOperation::Up);
	}
	else if (event->key() == Qt::Key::Key_Down)
	{
		moveCursor(QTextCursor::MoveOperation::Down);
	}

	// TODO: handle TAB focus switching properly

	// skip all keys but the letters and numbers
	else if (!((event->key() >= 0x30 && event->key() <= 0x39) || (event->key() >= 0x41 && event->key() <= 0x5a)))
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
	if (m_words.length() < 1)
	{
		// remove the "\n" at the end so that the box doesn't scroll down and hide some text
		if (this->toPlainText().endsWith("\n"))
		{
			this->setPlainText(this->toPlainText().remove(this->toPlainText().length() - 1, 1));
			// make sure the cursor remains at the end
			moveCursor(QTextCursor::MoveOperation::End);
		}
		clearFocus();
		emit messageToUser(tr("Done!"));
		emit done();
	}
}
