#include "MainWindow.h"

#include <QApplication>
#include <QPushButton>

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
