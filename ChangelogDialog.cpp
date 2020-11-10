#include "ChangelogDialog.h"

#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>

#include "maddy/parser.h"

ChangelogDialog::ChangelogDialog(QWidget *parent)
	: QDialog{ parent }
{
	QFile changelog{ ":/CHANGELOG.md" };
	changelog.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream ts{ &changelog };

	QString data;
	data.reserve(changelog.size());
	data = ts.readAll();

	std::stringstream stream{ data.toStdString() };
	maddy::Parser parser;

	// pointer because we don't want to clog up the memory with lots of text
	std::string *parsed = new std::string;
	*parsed = parser.Parse(stream);
	QString html{ parsed->c_str() };
	delete parsed;

	this->setWindowTitle(tr("Changelog"));

	auto changelogView = new QTextBrowser();
	changelogView->setHtml(html);
	changelogView->setOpenExternalLinks(true);

	auto ok = new QPushButton;
	ok->setText(tr("OK"));
	connect(ok, &QPushButton::clicked, this, &ChangelogDialog::accept);

	auto buttonRow = new QHBoxLayout;
	buttonRow->addStretch();
	buttonRow->addWidget(ok);

	auto layout = new QVBoxLayout;
	layout->addWidget(changelogView);
	layout->addLayout(buttonRow);

	this->setLayout(layout);

	// make this box a reasonable size for viewing the file
	// TODO: Make this independent of a hard-coded size
	this->resize(600, 600);
	this->show();
}
