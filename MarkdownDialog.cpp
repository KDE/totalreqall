// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com
// SPDX-License-Identifier: BSD-2-Clause

#include "MarkdownDialog.h"

#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QHBoxLayout>

MarkdownDialog::MarkdownDialog(QWidget *parent, QString title, QString text, bool modal)
	: QDialog{ parent }
{
	this->setWindowTitle(title);
	this->setModal(modal);

	auto markdownView = new QTextBrowser();
	markdownView->setHtml(text);
	markdownView->setOpenExternalLinks(true);

	auto ok = new QPushButton;
	ok->setText(tr("OK"));
	connect(ok, &QPushButton::clicked, this, &MarkdownDialog::accept);

	auto buttonRow = new QHBoxLayout;
	buttonRow->addStretch();
	buttonRow->addWidget(ok);

	auto layout = new QVBoxLayout;
	layout->addWidget(markdownView);
	layout->addLayout(buttonRow);

	this->setLayout(layout);

	// make this box a reasonable size for viewing the file
	// TODO: Make this independent of a hard-coded size
	this->resize(600, 600);
	this->show();
}
