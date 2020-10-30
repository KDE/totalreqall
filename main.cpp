// main.cpp: purpose of this file should be obvious
// Copyright 2020 Loren Burkholder.

#include <QApplication>
#include <QPushButton>

#include "MainWindow.h"
#include "MemorizeWidget.h"
#include "ChooseReferenceWidget.h"
#include "Bible.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MainWindow mainWindow;
	mainWindow.setWindowTitle(QApplication::tr("TotalReqall Bible Memory"));
	mainWindow.show();

	return a.exec();
}
