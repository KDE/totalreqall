// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-License-Identifier: BSD-2-Clause

#include "MainWindow.h"

#include "AppInfo.h"
#include "SettingsDialog.h"
#include <QActionGroup>
#include <QApplication>
#include <QDesktopServices>
#include <QFile>
#include <QIcon>
#include <QSettings>
#include <QTextStream>

MainWindow::MainWindow(QMainWindow *parent)
    : QMainWindow(parent),
      m_refChooser{ new ChooseReferenceWidget },
      m_welcomePage{ new WelcomePage },
      m_contentAdder{ new CustomContentAdder },
      m_savedLoader{ new SavedContentLoader }
{
    setWindowIcon(QIcon{ ":/resources/icons/TotalReqall.svg" });

    // set up the menus
    // file menu
    auto fileMenu = new QMenu{ tr("&File") };

    // settings action
    auto settings = new QAction{ tr("Settings...") };
    connect(settings, &QAction::triggered, this, [this]() {
        SettingsDialog s{ this };
        s.exec();
    });
    fileMenu->addAction(settings);

    // exit action
#ifndef Q_OS_WASM // Skip this on WASM
    auto exit = new QAction{ tr("Exit") };
    connect(exit, &QAction::triggered, this, &MainWindow::close);
    fileMenu->addAction(exit);
#endif

    // mode menu
    auto modeMenu = new QMenu{ tr("Mode") };

    auto saved = new QAction{ tr("&Saved") };
    auto bibleVerse = new QAction{ tr("&Bible verse") };
    auto custom = new QAction{ tr("&Custom content") };

    saved->setCheckable(true);
    bibleVerse->setCheckable(true);
    custom->setCheckable(true);

    auto modeGroup = new QActionGroup{ nullptr };
    modeGroup->addAction(saved);
    modeGroup->addAction(bibleVerse);
    modeGroup->addAction(custom);

    modeMenu->addAction(saved);
    modeMenu->addAction(bibleVerse);
    modeMenu->addAction(custom);

    // help menu
    auto helpMenu = new QMenu{ tr("&Help") };

    // online help
    auto onlineHelp = new QAction{ tr("Online help...") };
    connect(onlineHelp, &QAction::triggered, this, []() {
        QDesktopServices::openUrl(QUrl{ "https://lorendb.github.io/TotalReqall/help" });
    });
    helpMenu->addAction(onlineHelp);

    // out of the help, into the about section
    helpMenu->addSeparator();

    // about Qt
    auto aboutQt = new QAction{ tr("About Qt") };
    connect(aboutQt, &QAction::triggered, this, [this]() {
        QMessageBox::aboutQt(this);
    });
    helpMenu->addAction(aboutQt);

    // about this program
    auto about = new QAction{ tr("About") };
    connect(about, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, tr("About"),
                           TotalReqall::appName + tr(" version ") +
                               TotalReqall::appVersion.toString() +
                               tr("<br>Copyright © 2020 Loren Burkholder."
                                  "<br><br><a "
                                  "href=\"https://lorendb.github.io/TotalReqall\">https://"
                                  "lorendb.github.io/TotalReqall</a>"));
    });
    helpMenu->addAction(about);

    // add all menus
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(modeMenu);
    menuBar()->addMenu(helpMenu);

    connect(m_refChooser, &ChooseReferenceWidget::startMemorizer, this, &MainWindow::runMemorizer);
    connect(m_contentAdder, &CustomContentAdder::ok, this, &MainWindow::runMemorizer);
    connect(m_savedLoader, &SavedContentLoader::contentReady, this, &MainWindow::runMemorizer);

    setCentralWidget(m_welcomePage);

    connect(saved, &QAction::triggered, m_welcomePage, &WelcomePage::savedClicked);
    connect(m_welcomePage, &WelcomePage::savedClicked, this, [this, saved]() {
        takeCentralWidget();

        // make sure that all content is displayed
        m_savedLoader->refresh();
        setCentralWidget(m_savedLoader);
        saved->setChecked(true);
    });

    connect(bibleVerse, &QAction::triggered, m_welcomePage, &WelcomePage::bibleClicked);
    connect(m_welcomePage, &WelcomePage::bibleClicked, this, [this, bibleVerse]() {
        takeCentralWidget();
        setCentralWidget(m_refChooser);
        bibleVerse->setChecked(true);
    });

    connect(custom, &QAction::triggered, m_welcomePage, &WelcomePage::customClicked);
    connect(m_welcomePage, &WelcomePage::customClicked, this, [this, custom]() {
        takeCentralWidget();
        setCentralWidget(m_contentAdder);
        custom->setChecked(true);
    });

    connect(m_contentAdder, &CustomContentAdder::cancel, this, [this]() {
        takeCentralWidget();
        setCentralWidget(m_welcomePage);
    });

    connect(m_savedLoader, &SavedContentLoader::cancel, this, [this]() {
        takeCentralWidget();
        setCentralWidget(m_welcomePage);
    });

    // make this widget sized decently because the welcome page makes the window extremely small,
    // which leads to uncomfortable usage of the ChooseReferenceWidget and the CustomContentAdder
    // later on these sizes are pulled from my measurements of the default size of a
    // ChooseReferenceWidget, which I thought had a nice default size
    resize(415, 315);
}

MainWindow::~MainWindow() {}

void MainWindow::runMemorizer(const QString &content)
{
    m_saveFocusWidget = QApplication::focusWidget();
    m_saveCentralWidget = takeCentralWidget();

    if (m_memorizer != nullptr)
        delete m_memorizer;
    m_memorizer = new MemorizeWidget{ content };
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
