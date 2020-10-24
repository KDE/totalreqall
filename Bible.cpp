#include "Bible.h"

#include <string>

#include <QTextStream>

Bible::Bible(QObject *parent)
    : QObject(parent),
      m_bibleFile { new QFile{ ":/resources/bible.txt" } }
{
	m_booksOfBible << "Genesis" << "Exodus" << "Leviticus" << "Numbers" << "Deuteronomy"
	      << "Joshua" << "Judges" << "Ruth" << "1 Samuel" << "2 Samuel"
	      << "1 Kings" << "2 Kings" << "1 Chronicles" << "2 Chronicles" << "Ezra"
	      << "Nehemiah" << "Esther" << "Job" << "Psalms" << "Proverbs"
	      << "Ecclesiasties" << "Song of Solomon" << "Isaiah" << "Jeremiah"
	      << "Lamentations" << "Ezekiel" << "Daniel" << "Hosea" << "Joel" << "Amos"
	      << "Obadiah" << "Jonah" << "Micah" << "Nahum" << "Habakkuk"
	      << "Zephaniah" << "Haggai" << "Zechariah" << "Malachi" << "Matthew"
	      << "Mark" << "Luke" << "John" << "Acts" << "Romans" << "1 Corinthians"
	      << "2 Corinthians" << "Galatians" << "Ephesians" << "Philippians"
	      << "Colossians" << "1 Thessalonians" << "2 Thessalonians" << "1 Timothy"
	      << "2 Timothy" << "Titus" << "Philemon" << "Hebrews" << "James"
	      << "1 Peter" << "2 Peter" << "1 John" << "2 John" << "3 John" << "Jude"
	      << "Revelation";

	QTextStream ts{ m_bibleFile };
	m_bibleData = ts.readAll().toStdString();

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
	// Set up the book string.
	QString bookBase{ bookName };
	bookBase.append(" %1:");
	QString book;

	// initial setup of the position
	auto pos = m_bibleData.find(bookName.toStdString(), 0);

	// the check for npos will occur in the loop so we can access i for our return value
	for (int i = 1; ; ++i)
	{
		book = bookBase;
		book.arg(i);
		pos = m_bibleData.find(book.toStdString(), pos);

		// end of the line
		if (pos == std::string::npos)
			return i - 1;
	}
}

int Bible::scrapeVersesPerChapter(const QString &bookName, const int chapter)
{
	// set up the book & chapter string (e.g. "Genesis 23:")
	// N.B. the colon is necessary to prevent situations like a search for "Isaiah 3" returning
	// the positions of Isaiah 30, 31, 32, etc. etc.
	QString bookChapter{ bookName };
	bookChapter.append(" %1:").arg(chapter);

	// initial setup of the position
	auto pos = m_bibleData.find(bookChapter.toStdString(), 0);
	int numVerses = (pos != std::string::npos) ? 1 : 0;

	while (pos != std::string::npos)
	{
		m_bibleData.find(bookChapter.toStdString(), pos);
		++numVerses;
	}

	return numVerses;
}


// *** This is on hold in favor of simply scraping data ***

//m_chaptersPerBook.insert("Genesis", 50);
//m_chaptersPerBook.insert("Exodus", 40);
//m_chaptersPerBook.insert("Leviticus", 27);
//m_chaptersPerBook.insert("Numbers", 36);
//m_chaptersPerBook.insert("Deuteronomy", 34);
//m_chaptersPerBook.insert("Joshua", 24);
//m_chaptersPerBook.insert("Judges", 21);
//m_chaptersPerBook.insert("Ruth", 4);
//m_chaptersPerBook.insert("1 Samuel", 31);
//m_chaptersPerBook.insert("2 Samuel", 24);
//m_chaptersPerBook.insert("1 Kings", 22);
//m_chaptersPerBook.insert("2 Kings", 25);
//m_chaptersPerBook.insert("1 Chronicles", 29);
//m_chaptersPerBook.insert("2 Chronicles", 36);
//m_chaptersPerBook.insert("Ezra", 10);
//m_chaptersPerBook.insert("Nehemiah", 13);
//m_chaptersPerBook.insert("Esther", 10);
//m_chaptersPerBook.insert("Job", 42);
//m_chaptersPerBook.insert("Psalms", 150);
//m_chaptersPerBook.insert("Proverbs", 31);
//m_chaptersPerBook.insert("Ecclesiasties", 12);
//m_chaptersPerBook.insert("Song of Solomon", 8);
//m_chaptersPerBook.insert("Isaiah", 66);
//m_chaptersPerBook.insert("Jeremiah", 52);
//m_chaptersPerBook.insert("Lamentations", 5);
//m_chaptersPerBook.insert("Ezekiel", 48);
//m_chaptersPerBook.insert("Daniel", 12);
//m_chaptersPerBook.insert("Hosea", 14);
//m_chaptersPerBook.insert("Joel", 3);
//m_chaptersPerBook.insert("Amos", 9);
//m_chaptersPerBook.insert("Obadiah", 1);
//m_chaptersPerBook.insert("Jonah", 4);
//m_chaptersPerBook.insert("Micah", 7);
//m_chaptersPerBook.insert("Nahum", 3);
//m_chaptersPerBook.insert("Habakkuk", 3);
//m_chaptersPerBook.insert("Zephaniah", 3);
//m_chaptersPerBook.insert("Haggai", 2);
//m_chaptersPerBook.insert("Zechariah", 14);
//m_chaptersPerBook.insert("Malachi", 4);
//m_chaptersPerBook.insert("Matthew", 28);
//m_chaptersPerBook.insert("Mark", 16);
//m_chaptersPerBook.insert("Luke", 24);
//m_chaptersPerBook.insert("John", 21);
//m_chaptersPerBook.insert("Acts", 28);
//m_chaptersPerBook.insert("Romans", 16);
//m_chaptersPerBook.insert("1 Corinthians", 16);
//m_chaptersPerBook.insert("2 Corinthians", 13);
//m_chaptersPerBook.insert("Galatians", 6);
//m_chaptersPerBook.insert("Ephesians", 6);
//m_chaptersPerBook.insert("Philippians", 4);
//m_chaptersPerBook.insert("Colossians", 4);
//m_chaptersPerBook.insert("1 Thessalonians", 5);
//m_chaptersPerBook.insert("2 Thessalonians", 3);
//m_chaptersPerBook.insert("1 Timothy", 6);
//m_chaptersPerBook.insert("2 Timothy", 4);
//m_chaptersPerBook.insert("Titus", 3);
//m_chaptersPerBook.insert("Philemon", 1);
//m_chaptersPerBook.insert("Hebrews", 13);
//m_chaptersPerBook.insert("James", 5);
//m_chaptersPerBook.insert("1 Peter", 5);
//m_chaptersPerBook.insert("2 Peter", 3);
//m_chaptersPerBook.insert("1 John", 5);
//m_chaptersPerBook.insert("2 John", 1);
//m_chaptersPerBook.insert("3 John", 1);
//m_chaptersPerBook.insert("Jude", 1);
//m_chaptersPerBook.insert("Revelation", 22);
