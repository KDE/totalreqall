// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com
// SPDX-License-Identifier: BSD-2-Clause

#ifndef CUSTOMCONTENTADDER_H
#define CUSTOMCONTENTADDER_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>

class CustomContentAdder : public QWidget
{
	Q_OBJECT
public:
	explicit CustomContentAdder(QWidget *parent = nullptr);

	QString getTitle();
	QString getContent();

signals:
	void ok();
	void cancel();

private:
	QLineEdit *m_title;
	QTextEdit *m_content;
};

#endif // CUSTOMCONTENTADDER_H
