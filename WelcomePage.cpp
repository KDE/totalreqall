// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com
// SPDX-License-Identifier: BSD-2-Clause

#include "WelcomePage.h"

#include <QVBoxLayout>
#include <QCommandLinkButton>
#include <QLabel>

WelcomePage::WelcomePage(QWidget *parent)
    : QWidget(parent)
{
	auto layout = new QVBoxLayout;

	auto bible = new QCommandLinkButton{ tr("Bible verse"), tr("Memorize a verse or verses from the Bible.")};
	auto custom = new QCommandLinkButton{ tr("Custom"),
	        tr("Enter some custom content to memorize.")};

	connect(bible, &QCommandLinkButton::clicked, this, &WelcomePage::bibleClicked);
	connect(custom, &QCommandLinkButton::clicked, this, &WelcomePage::customClicked);

	layout->addWidget(new QLabel{ tr("Choose what to memorize.")});
	layout->addWidget(bible);
	layout->addWidget(custom);

	this->setLayout(layout);
}
