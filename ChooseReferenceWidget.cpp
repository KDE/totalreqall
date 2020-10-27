#include "ChooseReferenceWidget.h"

ChooseReferenceWidget::ChooseReferenceWidget(QWidget *parent)
    : QWidget(parent),
      m_layout{ new QGridLayout{ this } },
      m_books{ new QComboBox },
      m_chapters{ new QComboBox },
      m_verses{ new QComboBox }
{
	QStringList books;
	books << "Genesis" << "Exodus" << "Leviticus" << "Numbers" << "Deuteronomy"
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

	m_books->insertItems(0, books);
	m_layout->addWidget(m_books, 0, 0);

	this->setLayout(m_layout);

	setUpBible();
}

void ChooseReferenceWidget::setUpBible()
{
	m_bible = new Bible;

	// TODO: start destruction trigger timer with appropriate callback
}

void ChooseReferenceWidget::freeBible()
{
	delete m_bible;

	// TODO: take care of the timer here as well
}

// TODO: is there a way to simply de-allocate the Bible string instead?
