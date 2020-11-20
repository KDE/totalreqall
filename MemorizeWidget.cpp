// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com
// SPDX-License-Identifier: BSD-2-Clause

#include "MemorizeWidget.h"

#include <map>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QStringList>

QSet<QString> MemorizeWidget::s_memorizedContent = QSet<QString>();

MemorizeWidget::MemorizeWidget(QString memorizeContent, QWidget *parent)
	: QWidget{ parent },
      m_layout{ new QGridLayout{ this } },
      m_endSession{ new QPushButton },
      m_content{ memorizeContent },
      m_endMemorizerTimer{ new QTimer }
{
	// clear the status bar
	emit newStatus("");

	if (!MemorizeWidget::s_memorizedContent.contains(m_content))
		m_difficulty = Difficulty::Easy; // completely memorize this
	else
		m_difficulty = Difficulty::Hard; // we know this already

	// set up the widgets...
	m_memorizeEdit = new MemorizeEdit{ m_content, m_difficulty };
	m_endSession->setText(tr("Stop memorizing"));

	connect(m_memorizeEdit, &MemorizeEdit::done, this, &MemorizeWidget::editDone);
	connect(m_endSession, &QPushButton::clicked, this, &MemorizeWidget::done);
	connect(m_memorizeEdit, &MemorizeEdit::messageToUser, this, &MemorizeWidget::newStatus);

	// ...and add them to the layout
	auto btnRow = new QHBoxLayout;
	btnRow->insertStretch(0);
	btnRow->insertWidget(1, m_endSession);
	m_layout->addWidget(m_memorizeEdit, 0, 0);
	m_layout->addLayout(btnRow, 1, 0);

	// now we can set the layout
	this->setLayout(m_layout);
}

MemorizeWidget::~MemorizeWidget()
{
}

void MemorizeWidget::editDone()
{
	// remove this connection (now we want to go to the next level)
	disconnect(m_endSession, &QPushButton::clicked, this, &MemorizeWidget::done);
	connect(m_endSession, &QPushButton::clicked, this, &MemorizeWidget::nextLevel);
	m_endSession->setText(tr("Continue"));
	m_endSession->setFocus();
}

void MemorizeWidget::nextLevel()
{
	if (m_difficulty < Difficulty::Hard)
	{
		m_difficulty = static_cast<Difficulty>(m_difficulty + 1);
		m_endSession->setText(tr("Stop memorizing"));
		disconnect(m_endSession, &QPushButton::clicked, this, &MemorizeWidget::nextLevel);
		connect(m_endSession, &QPushButton::clicked, this, &MemorizeWidget::done);

		delete m_memorizeEdit;
		m_memorizeEdit = new MemorizeEdit{ m_content, m_difficulty };
		connect(m_memorizeEdit, &MemorizeEdit::done, this, &MemorizeWidget::editDone);
		m_layout->addWidget(m_memorizeEdit, 0, 0);
		m_memorizeEdit->setFocus();
	}
	else
	{
		// register this as having been memorized in this session
		s_memorizedContent.insert(m_content);

		m_endSession->setText(tr("Continue"));
		emit done();
	}
}
