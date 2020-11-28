// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com
// SPDX-License-Identifier: BSD-2-Clause

#include "CustomContentAdder.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>

CustomContentAdder::CustomContentAdder(QWidget *parent)
    : QWidget(parent),
      m_title{ new QLineEdit },
      m_content{ new QTextEdit }
{
    m_content->setAcceptRichText(false);

    auto buttons = new QDialogButtonBox{ QDialogButtonBox::Ok | QDialogButtonBox::Cancel };
    connect(buttons->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, [this]() {
        QSettings settings;
        settings.beginGroup("savedContent");
        settings.beginGroup("custom");
        settings.setValue(m_title->text(), m_content->toPlainText());
        settings.endGroup();
        settings.endGroup();
    });
    connect(buttons->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, [this]() {
        emit ok(m_content->toPlainText());
    });
    connect(buttons->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this,
            &CustomContentAdder::cancel);

    // A note about the layouts: I could not get the QFormLayout to automatically expand to fill all
    // available space, so the QVBoxLayout is used to prevent lots of empty space between the
    // OK/Cancel buttons and the bottom of the window
    auto form = new QFormLayout;
    form->addRow(new QLabel{ tr("Title:") }, m_title);
    form->addRow(new QLabel{ tr("Content:") }, m_content);

    auto layout = new QVBoxLayout;
    layout->addLayout(form);
    layout->addWidget(buttons);

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
