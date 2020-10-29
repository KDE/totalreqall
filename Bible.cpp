#include "Bible.h"

#include <string>
#include <fstream>

#include <QTextStream>
#include <QByteArray>
#include <QDebug>

Bible::Bible(QObject *parent)
    : QObject(parent),
      m_bibleFile { new QFile{ ":/resources/bible.txt" } },
      m_freeTimer{ new QTimer }
{
	// the timer needs to only run once
	m_freeTimer->setSingleShot(true);
	m_freeTimer->setInterval(1500);
	connect(m_freeTimer, &QTimer::timeout, this, &Bible::freeData);

	if (!m_bibleFile->open(QIODevice::ReadOnly | QIODevice::Text))
		qDebug() << tr("Failed to open :/resources/bible.txt");
	else
		readData();
}

Bible::~Bible()
{
	freeData();
}

// Note: The logic used here may be a weird way of doing things, but it's the
// best way I can think of to do it. Basically, given a string (e.g. "Mark") we
// set up a base string "Mark %1:", then copy the string and call .arg(i) where i
// starts at 1 and then increments until there are no results found, at which point
// we return (i - 1), since the last value of i is one more than the number of
// chapters.
// Since "John" occurs before "1 John", etc, we don't need to include logic to
// ensure that "John" doesn't end up in 1, 2, or 3 John.
int Bible::scrapeChaptersPerBook(const QString &bookName)
{
	readData();
	// Set up the book string.
	QString bookBase{ bookName };
	bookBase.append(" %1:");
	QString book;

	// initial setup of the position
	int pos = 0;
	int numChapters = 0;

	// the check for npos will occur in the loop so we can access i for our return value
	do
	{
		book = bookBase;
		// the + 1 is because we're checking for the next chapter (e.g. we know 1, let's see if 2 exists)
		book = book.arg(numChapters + 1);
		pos = m_bibleData.indexOf(book, pos, Qt::CaseInsensitive);
	} while (pos != -1 && ++numChapters);

	return numChapters;
}

int Bible::scrapeVersesPerChapter(const QString &bookName, const int chapter)
{
	QString chap{ "%1" };
	return this->scrapeVersesPerChapter(bookName, chap.arg(chapter));
}

int Bible::scrapeVersesPerChapter(const QString &bookName, const QString &chapter)
{
	readData();
	// set up the book & chapter string (e.g. "Genesis 23:")
	// N.B. the colon is necessary to prevent situations like a search for "Isaiah 3" returning
	// the positions of Isaiah 30, 31, 32, etc. etc.
	QString bookChapterBase{ bookName + " " };
	bookChapterBase.append(chapter + ":%1");
	QString book;

	// initial setup of the position
	int pos = 0;
	int numVerses = 0;

	do
	{
		book = bookChapterBase;
		book = book.arg(numVerses + 1);
		pos = m_bibleData.indexOf(book, pos, Qt::CaseInsensitive);
	} while (pos != -1 && ++numVerses);

	return numVerses;
}

void Bible::readData()
{
	if (m_bibleData.size() == 0)
	{
		qDebug() << tr("Loading Bible");

		// move to the beginning of the file
		m_bibleFile->seek(0);
		m_bibleData.reserve(m_bibleFile->size());
		QTextStream ts{ m_bibleFile };
		m_bibleData = ts.readAll();
	}

	// give the user 3 minutes before we free the Bible
	m_freeTimer->start(180000);
}

void Bible::freeData()
{
	qDebug() << tr("Freeing Bible");
	m_bibleData = "";
	m_bibleData.shrink_to_fit();
	m_freeTimer->stop();
}

QString Bible::getVerseStringFromRef(const QString &reference)
{
	readData();
	int startPos = m_bibleData.indexOf(reference, 0);
	int endPos = m_bibleData.indexOf("\n", startPos);
	std::string temp = m_bibleData.toStdString();
	temp = temp.substr(startPos + reference.size() + 1, (endPos - startPos) - reference.size());
	QString verse = temp.c_str();
	return verse;
}

