#include "MemorizeEdit.h"

#include <string>

#include <QSettings>
#include <QRandomGenerator>

MemorizeEdit::MemorizeEdit(QString &memorizeContent, Difficulty difficulty, QWidget *parent)
    : QTextEdit{ parent },
      m_difficulty{ difficulty }
{
	QSettings settings;
	settings.beginGroup("MemorizeEdit");

	// the static_cast is necessary here
	m_errorAction = static_cast<ErrorAction>(settings.value("errorAction", ErrorAction::Redo).toInt());

	setAcceptRichText(true);
	setTabChangesFocus(true);
	setReadOnly(true);

	// add a space after each newline so that words break appropriately at newlines (e.g. "word1\n" "word2"
	// instead of "word1\nword2") and also change the "\n" to a "<br>" to play right with the HTML text
	// std::string is the easiest way to do this
	auto temp = memorizeContent.toStdString();

	for (std::string::size_type pos = temp.find("\n", 0); pos != std::string::npos; pos = temp.find("\n", pos))
		temp.replace(pos, 1, "<br> ");

	QString newMemContent{ temp.c_str() };
	m_words = newMemContent.split(" ", QString::SplitBehavior::SkipEmptyParts);

	m_numWords = m_words.size();
	setHtml(formattedEndString(m_difficulty));

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

		// Delete this word now so that calculations don't get messed up in formattedEndString()
		m_words.pop_front();

		setHtml(m_richText + formattedEndString(m_difficulty));

		emit messageToUser("Hint provided.");
	}

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
				emit messageToUser(tr("Oops, you messed up! Try again."));
				break;

			case ErrorAction::KeepGoing:
				m_richText += "<span style=\"color:red;\">" + m_words[0];

				if (m_richText.at(m_richText.length() - 1) != "\n")
					m_richText += " ";

				m_richText += "</span>";
				m_words.pop_front();
				setHtml(m_richText + formattedEndString(m_difficulty));

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
		if (m_richText.endsWith("<br> "))
			this->setHtml(m_richText.remove(m_richText.length() - 5, 5));
		else if (m_richText.endsWith("<br> </span>"))
			this->setHtml(m_richText.remove(m_richText.length() - 12, 12));

		emit messageToUser(tr("Done!"));
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
				formatted += "<span style=\"color:#8b8b8b;\">" + m_words[i - numWordsUsed] + " </span>";
			else
				formatted += "<span style=\"color:#00000000;\">" + m_words[i - numWordsUsed] + " </span>";
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
