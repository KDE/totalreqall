#include "MemorizeWidget.h"

#include <map>

#include <QGridLayout>
#include <QDebug>
#include <QStringList>

MemorizeWidget::MemorizeWidget(QString memorizeContent, QWidget *parent)
    : QWidget{ parent },
      m_layout{ new QGridLayout{ this } },
      m_endSession{ new QPushButton },
      m_memorizeEdit{ new MemorizeEdit{ memorizeContent } },
      m_endMemorizerTimer{ new QTimer }
{
	// clear the status bar
	emit newStatus("");

	// set up the widgets...
	m_endSession->setText(tr("Stop memorizing"));

	m_endMemorizerTimer->setInterval(1000);
	m_endMemorizerTimer->setSingleShot(true);

	// this delays the signal for 1 second so the user has a moment to look at the completed verse
//	connect(m_memorizeEdit, SIGNAL(done()), m_endMemorizerTimer, SLOT(start()));
//	connect(m_endMemorizerTimer, &QTimer::timeout, this, &MemorizeWidget::done);

	connect(m_memorizeEdit, &MemorizeEdit::done, this, &MemorizeWidget::editDone);
	connect(m_endSession, &QPushButton::clicked, this, &MemorizeWidget::done);
	connect(m_memorizeEdit, &MemorizeEdit::messageToUser, this, &MemorizeWidget::newStatus);

    // ...and add them to the layout
	m_layout->addWidget(new QLabel{ tr("Type the first letter of each word.") }, 0, 0);
	m_layout->addWidget(m_memorizeEdit, 1, 0);
	m_layout->addWidget(m_endSession, 2, 0);

    // now we can set the layout
	this->setLayout(m_layout);
}

MemorizeWidget::~MemorizeWidget()
{
}

void MemorizeWidget::editDone()
{
	m_endSession->setText(tr("Continue"));
}
