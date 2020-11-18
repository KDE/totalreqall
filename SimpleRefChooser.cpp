#include "SimpleRefChooser.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QStringList>

#include <swmgr.h>
#include <swmodule.h>
#include <versekey.h>
#include <markupfiltmgr.h>

SimpleRefChooser::SimpleRefChooser(QWidget *parent, const QString &book, const QString &chapter, const QString &startVerse, const QString &endVerse)
	: QDialog{ parent },
	  m_layout{ new QGridLayout{ this } },
      m_books{ new QComboBox },
      m_chapters{ new QComboBox },
      m_startVerses{ new QComboBox },
      m_endVerses{ new QComboBox },
      m_ok{ new QPushButton },
      m_cancel{ new QPushButton }
{
	m_ok->setText(tr("OK"));
	m_cancel->setText(tr("Cancel"));

	connect(m_ok, &QPushButton::clicked, this, &SimpleRefChooser::accept);
	connect(m_cancel, &QPushButton::clicked, this, &SimpleRefChooser::reject);

	// get the values for the books (chapters and verses will come later)
	sword::SWMgr mgr{ new sword::MarkupFilterMgr{ sword::FMT_PLAIN } };
	sword::SWModule *module = mgr.getModule("KJV");
	sword::VerseKey *key{ static_cast<sword::VerseKey *>(module->getKey()) };
	QStringList bookList;
	while (!key->popError())
	{
		bookList << key->getBookName();
		key->setBook(key->getBook() + 1);
	}
	m_books->insertItems(0, bookList);

	// connect the combos
	connect(m_books, SIGNAL(currentIndexChanged(int)), this, SLOT(updateChapterVerseValues()));
	connect(m_chapters, SIGNAL(currentIndexChanged(int)), this, SLOT(updateVerseValues()));
	connect(m_startVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEndVerseValues()));
	connect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEndVerseValues()));

	// we also need to save all info about the last verse
	connect(m_books, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSaveVerse()));
	connect(m_chapters, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSaveVerse()));
	connect(m_startVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSaveVerse()));
	connect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSaveVerse()));

	if (book != "" && chapter != "" && startVerse != "" && endVerse != "")
	{
		m_books->setCurrentText(book);
		m_chapters->setCurrentText(chapter);
		m_startVerses->setCurrentText(startVerse);
		m_endVerses->setCurrentText(endVerse);
	}

	updateChapterVerseValues();

	// ... and make sure that the combo boxes don't display ellipses (try removing this and then
	// viewing the chapters for Psalms if you think this is unnecessary!)
	m_chapters->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	m_startVerses->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	m_endVerses->setSizeAdjustPolicy(QComboBox::AdjustToContents);

	auto colon = new QLabel{ ":" };
	colon->setAlignment(Qt::AlignHCenter);
	auto dash = new QLabel{ "-" };
	dash->setAlignment(Qt::AlignHCenter);

	// set up the layout
	m_layout->addWidget(m_books, 0, 0);
	m_layout->addWidget(m_chapters, 0, 1);
	m_layout->addWidget(colon, 0, 2);
	m_layout->addWidget(m_startVerses, 0, 3);
	m_layout->addWidget(dash, 0, 4);
	m_layout->addWidget(m_endVerses, 0, 5);

	auto buttonRow = new QHBoxLayout;
	buttonRow->addStretch();
	buttonRow->addWidget(m_ok);
	buttonRow->addWidget(m_cancel);

	m_layout->addLayout(buttonRow, 1, 0, 1, 6);

	this->setLayout(m_layout);
}

void SimpleRefChooser::updateChapterVerseValues()
{
	// prevent lots of signals from being emitted
	disconnect(m_chapters, SIGNAL(currentIndexChanged(int)), this, SLOT(updateVerseValues()));
	disconnect(m_chapters, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSaveVerse()));

	// set up sword
	sword::SWMgr mgr{ new sword::MarkupFilterMgr{ sword::FMT_PLAIN } };
	sword::SWModule *module = mgr.getModule("KJV");
	sword::VerseKey *key{ static_cast<sword::VerseKey *>(module->getKey()) };
	key->setBookName(m_books->currentText().toStdString().c_str());

	// first take care of the chapter
	// get data to insert
	int chapters = key->getChapterMax();

	// make sure that we do not set the current index to -1, use the first item instead
	auto old = (m_chapters->currentIndex() == -1) ? 0 : m_chapters->currentIndex();

	// remove existing items
	m_chapters->clear();

	// insert the new data
	QStringList chapList;

	for (int i = 0; i < chapters; ++i)
	{
		QString temp{ "%1" };
		temp = temp.arg(i + 1);
		chapList.push_back(temp); // + 1 to ensure that we insert at the end
	}

	m_chapters->insertItems(0, chapList);

	// this triggers a call to updateVerseValues()
	if ((m_chapters->count() - 1) >= old)
		m_chapters->setCurrentIndex(old);
	else // use the closest thing to the old index that we have
		m_chapters->setCurrentIndex(m_chapters->count() - 1);

	updateVerseValues();
	updateSaveVerse();

	// reconnect the signals
	connect(m_chapters, SIGNAL(currentIndexChanged(int)), this, SLOT(updateVerseValues()));
	connect(m_chapters, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSaveVerse()));
}

void SimpleRefChooser::updateVerseValues()
{
	// prevent lots of signals from being emitted
	disconnect(m_startVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEndVerseValues()));
	disconnect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEndVerseValues()));
	disconnect(m_startVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSaveVerse()));
	disconnect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSaveVerse()));

	// set up sword
	sword::SWMgr mgr{ new sword::MarkupFilterMgr{ sword::FMT_PLAIN } };
	sword::SWModule *module = mgr.getModule("KJV");
	sword::VerseKey *key{ static_cast<sword::VerseKey *>(module->getKey()) };
	key->setBookName(m_books->currentText().toStdString().c_str());
	key->setChapter(m_chapters->currentText().toInt());

	// get data to insert
	int verses = key->getVerseMax();

	// make sure that we do not set the current index to -1, use the first item instead
	auto oldStart = (m_startVerses->currentIndex() == -1) ? 0 : m_startVerses->currentIndex();
	auto oldEnd = (m_endVerses->currentIndex() == -1) ? 0 : m_endVerses->currentIndex();

	// remove existing items
	m_startVerses->clear();
	m_endVerses->clear();

	// insert the new data
	QStringList verseList;

	for (int i = 0; i < verses; ++i)
	{
		QString temp{ "%1" };
		temp = temp.arg(i + 1);
		verseList.push_back(temp);
	}

	m_startVerses->insertItems(0, verseList);
	m_endVerses->insertItems(0, verseList);

	// make sure that the old index is still valid
	// - 1 because count isn't zero numbered but the index is
	if ((m_startVerses->count() - 1) >= oldStart)
		m_startVerses->setCurrentIndex(oldStart);
	else // use the closest thing to the old index that we have
		m_startVerses->setCurrentIndex(m_startVerses->count() - 1);

	if ((m_endVerses->count() - 1) >= oldEnd)
		m_endVerses->setCurrentIndex(oldEnd);
	else
		m_endVerses->setCurrentIndex(m_endVerses->count() - 1);

	// make sure that m_endVerses has a proper index
	updateEndVerseValues();

	updateSaveVerse();

	// reconnect the signals
	connect(m_startVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEndVerseValues()));
	connect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEndVerseValues()));
	connect(m_startVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSaveVerse()));
	connect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSaveVerse()));
}

void SimpleRefChooser::updateEndVerseValues()
{
	// prevent lots of signals from being emitted
	disconnect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEndVerseValues()));
	disconnect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSaveVerse()));

	if (m_startVerses->currentIndex() > m_endVerses->currentIndex())
		m_endVerses->setCurrentIndex(m_startVerses->currentIndex());

	updateSaveVerse();

	// reconnect the signals
	connect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateEndVerseValues()));
	connect(m_endVerses, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSaveVerse()));
}

void SimpleRefChooser::updateSaveVerse()
{
	m_book = m_books->currentText();
	m_chapter = m_chapters->currentText();
	m_startVerse = m_startVerses->currentText();
	m_endVerse = m_endVerses->currentText();
}
