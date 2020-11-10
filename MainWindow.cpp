#include "MainWindow.h"

#include <QSettings>
#include <QIcon>

#include "AppInfo.h"
#include "SettingsDialog.h"
#include "ChangelogDialog.h"

MainWindow::MainWindow(QMainWindow *parent)
    : QMainWindow(parent),
      m_refChooser{ new ChooseReferenceWidget }
{
	setWindowIcon(QIcon{ ":/resources/icons/TotalReqall.svg" });
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
	QAction *aboutMaddy = new QAction{ tr("About maddy") };
	QAction *changelog = new QAction{ tr("Changelog") };
	QAction *about = new QAction{ tr("About") };

	connect(aboutQt, &QAction::triggered, this, [this]() {
		QMessageBox::aboutQt(this);
	});
	connect(aboutMaddy, &QAction::triggered, this, [this]() {
		QMessageBox::information(this, tr("About maddy"), tr("maddy is a C++ Markdown to HTML <b>header-only</b> parser library.<br><br>"
		                                                     "<a href=\"https://github.com/progsource/maddy\">"
		                                                     "https://github.com/progsource/maddy</a>"));
	});
	connect(changelog, &QAction::triggered, this, [this]() {
		ChangelogDialog c{ this };
		c.exec();
	});
	connect(about, &QAction::triggered, this, &MainWindow::showAboutDlg);

	helpMenu->addAction(aboutQt);
	helpMenu->addAction(aboutMaddy);
	helpMenu->addSeparator();
	helpMenu->addAction(changelog);
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
}

void MainWindow::showAboutDlg()
{
	QMessageBox::about(this, tr("About"), TotalReqall::appName + tr(" version ") + TotalReqall::appVersion.toString()
	                   + tr("<br>Copyright © 2020 Loren Burkholder.")
	                   + tr("<br><br><a href=\"https://lorendb.github.io/TotalReqall\">https://lorendb.github.io/TotalReqall</a>"));
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
