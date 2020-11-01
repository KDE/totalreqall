// main.cpp: purpose of this file should be obvious
// Copyright 2020 Loren Burkholder.

#include <QApplication>
#include <QPushButton>
#include <QPoint>
#include <QScreen>
#include <QDebug>

#include "MainWindow.h"
#include "MemorizeWidget.h"
#include "ChooseReferenceWidget.h"
#include "Bible.h"

inline QPoint centerWindowOnScreen(int, int);

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MainWindow mainWindow;
	mainWindow.setWindowTitle(QApplication::tr("TotalReqall Bible Memory"));

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
