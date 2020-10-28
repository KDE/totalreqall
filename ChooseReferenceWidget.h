#ifndef CHOOSEREFERENCEWIDGET_H
#define CHOOSEREFERENCEWIDGET_H

#include <map>

#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QTimer>
#include <QStringList>

#include "Bible.h"

class ChooseReferenceWidget : public QWidget
{
	Q_OBJECT
public:
	explicit ChooseReferenceWidget(QWidget *parent = nullptr);

signals:

private slots:
	void updateChapterVerseValues();
	void updateVerseValues();

	void displayVerse();

private:
	QGridLayout *m_layout;
	QComboBox *m_books;
	QComboBox *m_chapters;
	QComboBox *m_verses;
	QPushButton *m_displayVerse;

	QStringList m_bookList;

	Bible *m_bible;
	QTimer *m_freeTimer;

	void setUpBible();
	void freeBible();
};

#endif // CHOOSEREFERENCEWIDGET_H
