#include "MemorizeEdit.h"

#include <string>

#include <QSettings>

MemorizeEdit::MemorizeEdit(QString &memorizeContent, QWidget *parent)
    : QTextEdit{ parent }
{
	QSettings settings;
	settings.beginGroup("MemorizeEdit");

	// the static_cast is necessary here
	m_errorAction = static_cast<ErrorAction>(settings.value("errorAction", ErrorAction::Redo).toInt());

	setAcceptRichText(true);
	setPlaceholderText(tr("Type the first letter of each word..."));
	setTabChangesFocus(true);

	// add a space after each newline so that words break appropriately at newlines (e.g. "word1\n" "word2"
	// instead of "word1\nword2")
	// std::string is the easiest way to do this
	auto temp = memorizeContent.toStdString();
	for (std::string::size_type pos = temp.find("\n", 0); pos != std::string::npos; pos = temp.find("\n", pos))
	{
		temp.replace(pos, 1, "\n ");
		pos += 2; // get past the \n we just inserted
	}

	QString newMemContent{ temp.c_str() };
	m_words = newMemContent.split(" ", QString::SplitBehavior::SkipEmptyParts);

	settings.endGroup(); // MemorizeEdit
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
		setHtml(m_richText);
		moveCursor(QTextCursor::MoveOperation::End);

		// Delete this word.
		m_words.pop_front();
		emit messageToUser("Hint provided.");
	}

	// let Qt handle text navigation events
	else if (event->key() == Qt::Key::Key_Left
	         || event->key() == Qt::Key::Key_Right
	         || event->key() == Qt::Key::Key_Up
	         || event->key() == Qt::Key::Key_Down
	         || event->key() == Qt::Key::Key_End
	         || event->key() == Qt::Key::Key_Home
	         || event->key() == Qt::Key::Key_PageUp
	         || event->key() == Qt::Key::Key_PageDown)
		QTextEdit::keyPressEvent(event);

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
			m_richText += m_words[0];
			if (m_richText.at(m_richText.length() - 1) != "\n")
				m_richText += " ";
			setText(m_richText);
			moveCursor(QTextCursor::MoveOperation::End);

			// Delete this word.
			m_words.pop_front();

			emit messageToUser("");
		}
		else
		{
			switch (m_errorAction)
			{
			case ErrorAction::Redo:
				emit messageToUser(tr("Oops, you messed up! Try again."));
				break;

			case ErrorAction::KeepGoing:
				m_richText += "<span style=\"color:red;\">" + m_words[0];
				if (m_richText.at(m_richText.length() - 1) != "\n")
					m_richText += " ";
				m_richText += "</span>";
				setHtml(m_richText);
				moveCursor(QTextCursor::MoveOperation::End);
				m_words.pop_front();
				emit messageToUser(tr("Oops, you messed up!"));
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
		if (this->toPlainText().endsWith("\n"))
		{
			this->setPlainText(this->toPlainText().remove(this->toPlainText().length() - 1, 1));
			// make sure the cursor remains at the end
			moveCursor(QTextCursor::MoveOperation::End);
		}
		emit messageToUser(tr("Done!"));
		emit done();
	}
}
