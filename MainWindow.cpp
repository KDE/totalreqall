// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-License-Identifier: BSD-2-Clause

#include "MainWindow.h"

#include <QApplication>
#include <QSettings>
#include <QIcon>
#include <QDesktopServices>
#include <QFile>
#include <QTextStream>

#include "AppInfo.h"
#include "SettingsDialog.h"

MainWindow::MainWindow(QMainWindow *parent)
	: QMainWindow(parent),
      m_refChooser{ new ChooseReferenceWidget },
      m_welcomePage{ new WelcomePage },
      m_contentAdder{ new CustomContentAdder }
{
	setWindowIcon(QIcon{ ":/resources/icons/TotalReqall.svg" });
	connect(m_refChooser, &ChooseReferenceWidget::startMemorizer, this, &MainWindow::runMemorizer);

	setCentralWidget(m_welcomePage);
	connect(m_welcomePage, &WelcomePage::bibleClicked, this, [this]() {
		takeCentralWidget();
		setCentralWidget(m_refChooser);
	});
	connect(m_welcomePage, &WelcomePage::customClicked, this, [this]() {
		takeCentralWidget();
		setCentralWidget(m_contentAdder);
	});

	connect(m_contentAdder, &CustomContentAdder::ok, this, [this]() {
		m_saveCentralWidget = takeCentralWidget();
		m_memorizer = new MemorizeWidget{ m_contentAdder->getContent() };
		setCentralWidget(m_memorizer);
		m_memorizer->focusMemorizer();
		connect(m_memorizer, &MemorizeWidget::newStatus, this, &MainWindow::setStatusMessage);
		connect(m_memorizer, &MemorizeWidget::done, this, &MainWindow::cleanUpMemorizer);
	});
	connect(m_contentAdder, &CustomContentAdder::cancel, this, [this]() {
		takeCentralWidget();
		setCentralWidget(m_welcomePage);
	});

	// set up the menus
	// file menu
	QMenu *fileMenu = new QMenu{ tr("&File") };

	// settings action
	QAction *settings = new QAction{ tr("Settings...") };
	connect(settings, &QAction::triggered, this, [this]()
	{
		SettingsDialog s{ this };
		s.exec();
	});
	fileMenu->addAction(settings);

	// exit action
#ifndef Q_OS_WASM // Skip this on WASM
	QAction *exit = new QAction{ tr("Exit") };
	connect(exit, &QAction::triggered, this, &MainWindow::close);
	fileMenu->addAction(exit);
#endif

	// help menu
	QMenu *helpMenu = new QMenu{ tr("&Help") };

	// online help
	QAction *onlineHelp = new QAction{ tr("Online help...") };
	connect(onlineHelp, &QAction::triggered, this, []() {
		QDesktopServices::openUrl(QUrl{ "https://lorendb.github.io/TotalReqall/help" });
	});
	helpMenu->addAction(onlineHelp);

	// out of the help, into the about section
	helpMenu->addSeparator();

	// about Qt
	QAction *aboutQt = new QAction{ tr("About Qt") };
	connect(aboutQt, &QAction::triggered, this, [this]()
	{
		QMessageBox::aboutQt(this);
	});
	helpMenu->addAction(aboutQt);

	// about this program
	QAction *about = new QAction{ tr("About") };
	connect(about, &QAction::triggered, this, [this]()
	{
		QMessageBox::about(this, tr("About"), TotalReqall::appName + tr(" version ") + TotalReqall::appVersion.toString()
						   + tr("<br>Copyright © 2020 Loren Burkholder."
								"<br><br><a href=\"https://lorendb.github.io/TotalReqall\">https://lorendb.github.io/TotalReqall</a>"));
	});
	helpMenu->addAction(about);

	// add all menus
	this->menuBar()->addMenu(fileMenu);
	this->menuBar()->addMenu(helpMenu);

	// make this widget sized decently because the welcome page makes the window extremely small, which leads to uncomfortable usage
	// of the ChooseReferenceWidget and the CustomContentAdder later on
	// these sizes are pulled from my measurements of the default size of a ChooseReferenceWidget, which I thought had a nice default size
	this->resize(415, 315);
}

MainWindow::~MainWindow()
{
}

void MainWindow::runMemorizer(const QString &verse)
{
	m_saveFocusWidget = QApplication::focusWidget();
	m_saveCentralWidget = takeCentralWidget();

	m_memorizer = new MemorizeWidget{ verse };
	m_memorizer->focusMemorizer();
	setCentralWidget(m_memorizer);

	connect(m_memorizer, &MemorizeWidget::newStatus, this, &MainWindow::setStatusMessage);
	connect(m_memorizer, &MemorizeWidget::done, this, &MainWindow::cleanUpMemorizer);
}

void MainWindow::cleanUpMemorizer()
{
	delete m_memorizer;
	m_memorizer = nullptr;

	setCentralWidget(m_saveCentralWidget);
	m_saveCentralWidget = nullptr;

	if (m_saveFocusWidget != nullptr)
	{
		m_saveFocusWidget->setFocus();
		m_saveFocusWidget = nullptr;
	}
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
