#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      m_layout{ new QGridLayout },
      m_refChooser{ new ChooseReferenceWidget }//,
      //m_memorizer{ new MemorizeWidget{ "Jesus wept." } }
{
    m_layout->addWidget(m_refChooser, 0, 0);
//    m_layout->addWidget(m_memorizer, 1, 0);

	this->setLayout(m_layout);

	this->setGeometry(100, 100, 400, 200);
}

MainWindow::~MainWindow()
{
}

