// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com>>
// SPDX-License-Identifier: BSD-2-Clause

#include "AppInfo.h"
#include "ChooseReferenceWidget.h"
#include "MainWindow.h"
#include "MemorizeWidget.h"
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

    QApplication a(argc, argv);
    MainWindow mainWindow;

    QSettings settings;
    if (settings.value("MainWindow/saveWinSize", true).toBool())
    {
        QVariant width = settings.value("MainWindow/width");
        QVariant height = settings.value("MainWindow/height");

        if (!width.isNull() && !height.isNull())
            mainWindow.resize(width.toInt(), height.toInt());
    }

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
