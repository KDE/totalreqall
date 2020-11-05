#ifndef CHOOSEREFERENCEWIDGET_H
#define CHOOSEREFERENCEWIDGET_H

#include <map>

#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QStringList>

#include "Bible.h"

enum VerseLoadOption
{
	Saved = 1,
	Random = 2,
	Set = 3,
};

class ChooseReferenceWidget : public QWidget
{
	Q_OBJECT
public:
	explicit ChooseReferenceWidget(QWidget *parent = nullptr);

signals:
	void signalRunMemorizer(const QString &);

private slots:
	void updateChapterVerseValues();
	void updateVerseValues();
	void updateEndVerseValues();
	void updateSaveVerse();

	void displayVerse();
	void runMemorizer();

private:
	QGridLayout *m_layout;
	QComboBox *m_books;
	QComboBox *m_chapters;
	QComboBox *m_startVerses;
	QComboBox *m_endVerses;
	QPushButton *m_runMemorizerBtn;
	QPushButton *m_displayVerseBtn;
	QTextEdit *m_verseDisplayBox;

	QStringList m_bookList;

	Bible *m_bible;
};

#endif // CHOOSEREFERENCEWIDGET_H
