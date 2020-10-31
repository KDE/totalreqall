#ifndef MainWindow_H
#define MainWindow_H

#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QWidget>
#include <QMainWindow>
#include <QGridLayout>

#include "ChooseReferenceWidget.h"
#include "MemorizeWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	MainWindow(QMainWindow *parent = nullptr);
	~MainWindow();

private:
	ChooseReferenceWidget *m_refChooser;
	// when a central widget needs removed but not deleted
	QWidget *m_saveCentralWidget = nullptr;
	MemorizeWidget *m_memorizer = nullptr;
    
private slots:
	void runMemorizer(const QString &);
	void cleanUpMemorizer();
	void showAboutDlg();
	void resizeToFit();
	void setStatusMessage(QString);

};
#endif // MainWindow_H
