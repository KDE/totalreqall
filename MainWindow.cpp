// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-License-Identifier: BSD-2-Clause

#include "MainWindow.h"

#include "AppInfo.h"
#include "SettingsDialog.h"
#include "UserSettings.h"
#include <KAboutApplicationDialog>
#include <KAboutData>
#include <KLocalizedString>
#include <QActionGroup>
#include <QApplication>
#include <QDesktopServices>
#include <QFile>
#include <QIcon>
#include <QSettings>
#include <QTextStream>

MainWindow::MainWindow(KMainWindow *parent)
    : KMainWindow{ parent },
      m_refChooser{ new ChooseReferenceWidget },
      m_welcomePage{ new WelcomePage },
      m_contentAdder{ new CustomContentAdder },
      m_savedLoader{ new SavedContentLoader }
{
    setWindowIcon(QIcon::fromTheme("TotalReqall", QIcon{ ":/resources/icons/TotalReqall.svg" }));

    // set up the menus
    // file menu
    auto fileMenu = new QMenu{ i18n("&File") };

    // settings action
    auto settings = new QAction{ i18n("Settings...") };
    connect(settings, &QAction::triggered, this, [this]() {
        SettingsDialog s{ this };
        s.exec();
    });
    fileMenu->addAction(settings);

    // exit action
#ifndef Q_OS_WASM // Skip this on WASM
    auto exit = new QAction{ i18n("Exit") };
    connect(exit, &QAction::triggered, this, &MainWindow::close);
    fileMenu->addAction(exit);
#endif

    // help menu
    auto helpMenu = new QMenu{ i18n("&Help") };

    // online help
    auto onlineHelp = new QAction{ i18n("Online help...") };
    connect(onlineHelp, &QAction::triggered, this, []() {
        QDesktopServices::openUrl(QUrl{ "https://lorendb.github.io/TotalReqall/help" });
    });
    helpMenu->addAction(onlineHelp);

    // out of the help, into the about section
    helpMenu->addSeparator();

    // about Qt
    auto aboutQt = new QAction{ i18n("About Qt") };
    connect(aboutQt, &QAction::triggered, this, [this]() {
        QMessageBox::aboutQt(this);
    });
    helpMenu->addAction(aboutQt);

    // about this program
    auto about = new QAction{ i18n("About") };
    connect(about, &QAction::triggered, this, [this]() {
        KAboutData aboutData{ "TotalReqall",
                              i18n("TotalReqall"),
                              TotalReqall::appVersion.toString(),
                              i18n("Memorize the Bible or custom content."),
                              KAboutLicense::BSDL,
                              i18n("Copyright (C) 2020 Loren Burkholder") };
        aboutData.addAuthor("Loren Burkholder", i18n("Creator, maintainer"),
                            "computersemiexpert@outlook.com");
        auto kdeAboutAppDlg =
            new KAboutApplicationDialog{ aboutData, KAboutApplicationDialog::NoOptions, this };
        kdeAboutAppDlg->exec();
    });
    helpMenu->addAction(about);

    // add all menus
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(helpMenu);

    connect(m_refChooser, &ChooseReferenceWidget::startMemorizer, this, &MainWindow::runMemorizer);
    connect(m_contentAdder, &CustomContentAdder::ok, this, &MainWindow::runMemorizer);
    connect(m_savedLoader, &SavedContentLoader::contentReady, this, &MainWindow::runMemorizer);

    setCentralWidget(m_welcomePage);

    connect(m_welcomePage, &WelcomePage::savedClicked, this, [this]() {
        takeCentralWidget();

        // make sure that all content is displayed
        m_savedLoader->refresh();
        setCentralWidget(m_savedLoader);
    });

    connect(m_welcomePage, &WelcomePage::bibleClicked, this, [this]() {
        takeCentralWidget();
        setCentralWidget(m_refChooser);
    });

    connect(m_welcomePage, &WelcomePage::customClicked, this, [this]() {
        takeCentralWidget();
        setCentralWidget(m_contentAdder);
    });

    connect(m_savedLoader, &SavedContentLoader::cancel, this, [this]() {
        takeCentralWidget();
        m_welcomePage->refresh();
        setCentralWidget(m_welcomePage);
    });

    connect(m_refChooser, &ChooseReferenceWidget::cancel, this, [this]() {
        takeCentralWidget();
        m_welcomePage->refresh();
        setCentralWidget(m_welcomePage);
    });

    connect(m_contentAdder, &CustomContentAdder::cancel, this, [this]() {
        takeCentralWidget();
        m_welcomePage->refresh();
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
    auto settings = UserSettings::global();

    if (settings->saveWinSize())
    {
        settings->setWindowWidth(width());
        settings->setWindowHeight(height());
    }

    QWidget::resizeEvent(event);
}
