// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com>>
// SPDX-License-Identifier: BSD-2-Clause

#include "AppInfo.h"
#include "ChooseReferenceWidget.h"
#include "MainWindow.h"
#include "MemorizeWidget.h"
#include "UserSettings.h"
#include <KI18n/KLocalizedString>
#include <QApplication>
#include <QPoint>
#include <QPushButton>
#include <QScreen>
#include <QSettings>

inline QPoint centerWindowOnScreen(int, int);

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName(TotalReqall::appName);
    QCoreApplication::setApplicationVersion(TotalReqall::appVersion.toString());
    //    QCoreApplication::setOrganizationName(TotalReqall::organizationName);
    //    QCoreApplication::setOrganizationDomain(TotalReqall::organizationDomain);

    KLocalizedString::setApplicationDomain(TotalReqall::appName.toStdString().c_str());

    // make sure that anything accessing the user settings actually gets data
    // The problem is that the global variable gets created before the app info
    // is set; therefore, unless refresh() is called, all settings are set to
    // the default value (or to a null QVariant).
    // This statement needs to come before any access of the UserSettings to prevent logic errors.
    UserSettings::global()->refresh();

    QApplication a(argc, argv);
    MainWindow mainWindow;

    auto settings = UserSettings::global();
    if (settings->saveWinSize())
        mainWindow.resize(settings->windowWidth(), settings->windowHeight());

    // calling move() before show() doesn't seem to work properly
    mainWindow.show();
    mainWindow.move(centerWindowOnScreen(mainWindow.width(), mainWindow.height()));

    return a.exec();
}

inline QPoint centerWindowOnScreen(int w, int h)
{
    auto size = QApplication::primaryScreen()->geometry();
    return QPoint{ (size.width() - w) / 2, (size.height() - h) / 2 };
}
