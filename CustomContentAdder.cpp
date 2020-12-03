// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com
// SPDX-License-Identifier: BSD-2-Clause

#include "CustomContentAdder.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QHBoxLayout>
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
    m_content->setTabChangesFocus(true);

    auto memorize = new QPushButton{ i18n("Memorize") };
    memorize->setIcon(QIcon::fromTheme("go-next"));
    connect(memorize, &QPushButton::clicked, this, &CustomContentAdder::saveItem);
    connect(memorize, &QPushButton::clicked, this, [this]() {
        emit ok(m_content->toPlainText());
    });

    auto back = new QPushButton{ i18n("Back") };
    back->setIcon(QIcon::fromTheme("go-previous"));
    connect(back, &QPushButton::clicked, this, &CustomContentAdder::cancel);

    auto save = new QPushButton{ i18n("Save") };
    save->setIcon(QIcon::fromTheme("document-save"));
    connect(save, &QPushButton::clicked, this, &CustomContentAdder::saveItem);

    auto buttons = new QHBoxLayout;
    buttons->addWidget(back);
    buttons->addStretch();
    buttons->addWidget(save);
    buttons->addWidget(memorize);

    // A note about the layouts: I could not get the QFormLayout to automatically expand to fill all
    // available space, so the QVBoxLayout is used to prevent lots of empty space between the
    // OK/Cancel buttons and the bottom of the window
    auto form = new QFormLayout;
    form->addRow(new QLabel{ i18n("Title:") }, m_title);
    form->addRow(new QLabel{ i18n("Content:") }, m_content);

    auto layout = new QVBoxLayout;
    layout->addLayout(form);
    layout->addLayout(buttons);

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

void CustomContentAdder::saveItem()
{
    QSettings settings;
    settings.beginGroup("savedContent");
    settings.beginGroup("custom");
    settings.setValue(m_title->text(), m_content->toPlainText());
    settings.endGroup();
    settings.endGroup();
}
