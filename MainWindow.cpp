#include "MainWindow.h"

#include <QSettings>

#include "AppInfo.h"
#include "SettingsDialog.h"

MainWindow::MainWindow(QMainWindow *parent)
    : QMainWindow(parent),
      m_refChooser{ new ChooseReferenceWidget }
{
	connect(m_refChooser, &ChooseReferenceWidget::signalRunMemorizer, this, &MainWindow::runMemorizer);

	setCentralWidget(m_refChooser);

	// set up the menus
	// file menu
	QMenu *fileMenu = new QMenu{ tr("&File") };

	QAction *settings = new QAction{ tr("Settings...") };
	QAction *exit = new QAction{ tr("Exit") };

	connect(settings, &QAction::triggered, this, &MainWindow::showSettingsDialog);
	connect(exit, &QAction::triggered, this, &MainWindow::close);

	fileMenu->addAction(settings);
	fileMenu->addAction(exit);

	// help menu
	QMenu *helpMenu = new QMenu{ tr("&Help") };

	QAction *aboutQt = new QAction{ tr("About Qt") };
	QAction *about = new QAction{ tr("About") };

	connect(aboutQt, &QAction::triggered, this, [this](){ QMessageBox::aboutQt(this); });
	connect(about, &QAction::triggered, this, &MainWindow::showAboutDlg);

	helpMenu->addAction(aboutQt);
	helpMenu->addAction(about);

	// add all menus
	this->menuBar()->addMenu(fileMenu);
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
	m_memorizer->focusMemorizer();

	connect(m_memorizer, &MemorizeWidget::newStatus, this, &MainWindow::setStatusMessage);
	connect(m_memorizer, &MemorizeWidget::done, this, &MainWindow::cleanUpMemorizer);
}

void MainWindow::cleanUpMemorizer()
{
	delete m_memorizer;
	setCentralWidget(m_saveCentralWidget);
	m_saveCentralWidget = nullptr;
	statusBar()->showMessage(tr("Please choose a verse."));
}

void MainWindow::showAboutDlg()
{
	QMessageBox::about(this, tr("About"), TotalReqall::appName + " version " + TotalReqall::appVersion.toString());
}

void MainWindow::resizeToFit()
{
	resize(sizeHint());
}

void MainWindow::setStatusMessage(QString message)
{
	this->statusBar()->showMessage(message);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
	QSettings settings;

	if (settings.value("MainWindow/saveWinSize", true).toBool())
	{
		settings.setValue("MainWindow/width", event->size().width());
		settings.setValue("MainWindow/height", event->size().height());
	}

	QWidget::resizeEvent(event);
}

void MainWindow::showSettingsDialog()
{
	SettingsDialog s;
	s.setModal(true);
	s.exec();
}
