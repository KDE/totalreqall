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

#include "Bible.h"

class ChooseReferenceWidget : public QWidget
{
	Q_OBJECT
public:
	explicit ChooseReferenceWidget(QWidget *parent = nullptr);

signals:

private:
	QGridLayout *m_layout;
	QComboBox *m_books;
	QComboBox *m_chapters;
	QComboBox *m_verses;

	Bible *m_bible;
	QTimer *m_freeTimer;

	void setUpBible();
	void freeBible();

	// here we store our data about the sizes of each book & chapter

};

#endif // CHOOSEREFERENCEWIDGET_H
