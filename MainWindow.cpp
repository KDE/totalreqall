// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com
// SPDX-License-Identifier: BSD-2-Clause

#include "MainWindow.h"

#include <QApplication>
#include <QSettings>
#include <QIcon>
#include <QDesktopServices>
#include <QFile>
#include <QTextStream>

#include <cmark.h>

#include "AppInfo.h"
#include "SettingsDialog.h"
#include "MarkdownDialog.h"

MainWindow::MainWindow(QMainWindow *parent)
	: QMainWindow(parent),
	  m_refChooser{ new ChooseReferenceWidget }
{
	setWindowIcon(QIcon{ ":/resources/icons/TotalReqall.svg" });
	connect(m_refChooser, &ChooseReferenceWidget::startMemorizer, this, &MainWindow::runMemorizer);

	setCentralWidget(m_refChooser);

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

	// out of the help, into the about libraries
	helpMenu->addSeparator();

	// about Qt
	QAction *aboutQt = new QAction{ tr("About Qt") };
	connect(aboutQt, &QAction::triggered, this, [this]()
	{
		QMessageBox::aboutQt(this);
	});
	helpMenu->addAction(aboutQt);

	// now into the about the program section
	helpMenu->addSeparator();

	// changelog
	QAction *changelog = new QAction{ tr("Changelog") };
	connect(changelog, &QAction::triggered, this, [this]()
	{
		QFile changelog{ ":/CHANGELOG.md" };
		if (changelog.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QTextStream ts{ &changelog };

			QString data;
			data.reserve(changelog.size());
			data = ts.readAll();

			QString html = cmark_markdown_to_html(data.toStdString().c_str(), data.length(), 0);

			MarkdownDialog dlg{ this, tr("Changelog"), html };
			dlg.exec();
		}
		else
			QMessageBox::information(this, tr("Couldn't load file"), tr("The file ") + changelog.fileName() + tr(" could not be opened."));
	});
	helpMenu->addAction(changelog);

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
}

MainWindow::~MainWindow()
{
}

void MainWindow::runMemorizer(const QString &verse)
{
	m_saveFocusWidget = QApplication::focusWidget();
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
	m_saveFocusWidget->setFocus();
	m_saveCentralWidget = nullptr;
	m_saveFocusWidget = nullptr;
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
