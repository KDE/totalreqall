#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QGridLayout>

#include "ChooseReferenceWidget.h"
#include "MemorizeWidget.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:
	QGridLayout *m_layout;

	ChooseReferenceWidget *m_refChooser;
	MemorizeWidget *m_memorizer = nullptr;
    
private slots:
	void runMemorizer(const QString &);
	void cleanUpMemorizer();

};
#endif // MAINWINDOW_H
