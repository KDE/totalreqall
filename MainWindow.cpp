#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      m_layout{ new QGridLayout },
      m_refChooser{ new ChooseReferenceWidget }
{
	connect(m_refChooser, &ChooseReferenceWidget::signalRunMemorizer, this, &MainWindow::runMemorizer);

	m_layout->addWidget(m_refChooser, 0, 0);

	this->setLayout(m_layout);
	this->setGeometry(100, 100, 400, 200);
}

MainWindow::~MainWindow()
{
}

void MainWindow::runMemorizer(const QString &verse)
{
	// TODO: remove m_refChooser from the layout or hide it somehow
	m_layout->removeWidget(m_refChooser);
	m_refChooser->hide();
	m_memorizer = new MemorizeWidget{ verse };
	m_layout->addWidget(m_memorizer, 0, 0);

	connect(m_memorizer, &MemorizeWidget::done, this, &MainWindow::cleanUpMemorizer);
}

void MainWindow::cleanUpMemorizer()
{
	// TODO: remove the memorizer from the layout
	// TODO: disconnect the connection
	m_layout->removeWidget(m_memorizer);
	delete m_memorizer;
	m_layout->addWidget(m_refChooser, 0, 0);
	m_refChooser->show();
}
