// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com
// SPDX-License-Identifier: BSD-2-Clause

#include "CustomContentAdder.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>

CustomContentAdder::CustomContentAdder(QWidget *parent)
    : QWidget(parent),
      m_title{ new QLineEdit },
      m_content{ new QTextEdit }
{
    m_content->setAcceptRichText(false);

    auto buttons = new QDialogButtonBox{ QDialogButtonBox::Ok | QDialogButtonBox::Cancel };
    connect(buttons->button(QDialogButtonBox::Ok), &QPushButton::clicked, this,
            &CustomContentAdder::ok);
    connect(buttons->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this,
            &CustomContentAdder::cancel);

    auto layout = new QFormLayout;
    layout->addRow(new QLabel{ tr("Title:") }, m_title);
    layout->addRow(new QLabel{ tr("Content:") }, m_content);
    layout->addRow(buttons);

    setLayout(layout);
}

QString CustomContentAdder::getTitle()
{
    return m_title->text();
}

QString CustomContentAdder::getContent()
{
    return m_content->toPlainText();
}
