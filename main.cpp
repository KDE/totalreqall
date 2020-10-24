#include "MainWindow.h"

#include <QApplication>
#include <QPushButton>

#include "MemorizeWidget.h"
#include "ChooseReferenceWidget.h"
#include "Bible.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Bible bible;

    MainWindow mainWindow;
    mainWindow.show();

    return a.exec();
}
