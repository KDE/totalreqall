// main.cpp: purpose of this file should be obvious
// Copyright 2020 Loren Burkholder.

#include <QApplication>
#include <QPushButton>
#include <QPoint>
#include <QScreen>
#include <QSettings>

#include "AppInfo.h"
#include "MainWindow.h"
#include "MemorizeWidget.h"
#include "ChooseReferenceWidget.h"
#include "Bible.h"

inline QPoint centerWindowOnScreen(int, int);

int main(int argc, char *argv[])
{
	QCoreApplication::setApplicationName(TotalReqall::appName);
	QCoreApplication::setApplicationVersion(TotalReqall::version);
	QCoreApplication::setOrganizationName(TotalReqall::organizationName);
	QCoreApplication::setOrganizationDomain(TotalReqall::organizationDomain);

	QApplication a(argc, argv);

	MainWindow mainWindow;

	// TODO: Somebody with a Mac should figure out whether this is needed
	mainWindow.setUnifiedTitleAndToolBarOnMac(true);

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
