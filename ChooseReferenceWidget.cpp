#include "ChooseReferenceWidget.h"

ChooseReferenceWidget::ChooseReferenceWidget(QWidget *parent)
    : QWidget(parent),
      m_layout{ new QGridLayout{ this } },
      m_books{ new QComboBox },
      m_chapters{ new QComboBox },
      m_verses{ new QComboBox },
      m_bible{ new Bible },
      m_displayVerse{ new QPushButton },
      m_freeTimer{ new QTimer }
{
	// the timer needs to only run once
	m_freeTimer->setSingleShot(true);

	// set this up first so that we can scrape data for the combo boxes
	setUpBible();

	// set up the first combo box manually and scrape the other 2
	// maybe the books could be scraped but this way seems easier ATM
	m_bookList << "Genesis" << "Exodus" << "Leviticus" << "Numbers" << "Deuteronomy"
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
	m_books->insertItems(0, m_bookList);
	updateChapterVerseValues();

	// ... and make sure that the combo boxes don't display ellipses (try removing this and then
	// viewing the chapters for Psalms if you think this is unnecessary!)
	m_chapters->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	m_verses->setSizeAdjustPolicy(QComboBox::AdjustToContents);

	// update the combos as appropriate
	connect(m_books, SIGNAL(currentIndexChanged(int)), this, SLOT(updateChapterVerseValues()));
	connect(m_chapters, SIGNAL(currentIndexChanged(int)), this, SLOT(updateVerseValues()));

	m_displayVerse->setText("Display selected verse");
	connect(m_displayVerse, SIGNAL(clicked(bool)), this, SLOT(displayVerse()));

	// set up the layout
	m_layout->addWidget(m_books, 0, 0);
	m_layout->addWidget(m_chapters, 0, 1);
	m_layout->addWidget(m_verses, 0, 2);
	m_layout->addWidget(m_displayVerse, 1, 0);

	this->setLayout(m_layout);
}

void ChooseReferenceWidget::updateChapterVerseValues()
{
	// first take care of the chapter
	// get data to insert
	int chapters = m_bible->scrapeChaptersPerBook(m_books->currentText());

	// remove existing items
	m_chapters->clear();

	// insert the new data
	for (int i = 0; i < chapters; ++i)
	{
		QString temp{ "%1" };
		temp = temp.arg(i + 1);
		m_chapters->insertItem(m_chapters->count() + 1, temp); // + 1 to ensure that we insert at the end
	}

	// now take care of the verses
	updateVerseValues();
}

void ChooseReferenceWidget::updateVerseValues()
{
	// get data to insert
	int verses = m_bible->scrapeVersesPerChapter(m_books->currentText(), m_chapters->currentText());

	// remove existing items
	m_verses->clear();

	// insert the new data
	for (int i = 0; i < verses; ++i)
	{
		QString temp{ "%1" };
		temp = temp.arg(i + 1);
		m_verses->insertItem(m_verses->count() + 1, temp);
	}
}

void ChooseReferenceWidget::setUpBible()
{
	m_bible->readData();

	// give the user 3 minutes
	m_freeTimer->start(180000);
}

void ChooseReferenceWidget::freeBible()
{
	m_bible->freeData();
	m_freeTimer->stop();
}

void ChooseReferenceWidget::displayVerse()
{
	this->m_layout->addWidget(new QLabel{ "TODO: insert content here" }, 2, 0);
}
