#include "MainWindow.h"

MainWindow::MainWindow(QMainWindow *parent)
    : QMainWindow(parent),
      m_refChooser{ new ChooseReferenceWidget }
{
	connect(m_refChooser, &ChooseReferenceWidget::signalRunMemorizer, this, &MainWindow::runMemorizer);

	setCentralWidget(m_refChooser);

	// set up the help menu
	QMenu *helpMenu = new QMenu{ tr("&Help") };

	QAction *aboutQt = new QAction{ tr("About Qt") };
	QAction *about = new QAction{ tr("About") };

	connect(aboutQt, &QAction::triggered, this, [this](){ QMessageBox::aboutQt(this); });
	connect(about, &QAction::triggered, this, &MainWindow::showAboutDlg);

	helpMenu->addAction(aboutQt);
	helpMenu->addAction(about);

	// add all menus
	this->menuBar()->addMenu(helpMenu);

	// set up the status bar
	this->statusBar()->showMessage(tr("Please choose a verse."));
}

MainWindow::~MainWindow()
{
}

void MainWindow::runMemorizer(const QString &verse)
{
	m_saveCentralWidget = takeCentralWidget();
	m_memorizer = new MemorizeWidget{ verse };
	setCentralWidget(m_memorizer);

	connect(m_memorizer, &MemorizeWidget::done, this, &MainWindow::cleanUpMemorizer);
}

void MainWindow::cleanUpMemorizer()
{
	delete m_memorizer;
	setCentralWidget(m_saveCentralWidget);
	m_saveCentralWidget = nullptr;
}

void MainWindow::showAboutDlg()
{
	QMessageBox dlg{ QMessageBox::Icon::Information, tr("About"), tr("TotalReqall version x.x.x") };
	dlg.exec();
}
