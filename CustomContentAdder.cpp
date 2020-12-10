// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com
// SPDX-License-Identifier: BSD-2-Clause

#include "CustomContentAdder.h"

#include <KLocalizedString>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFile>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QStandardPaths>
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
    QFile file{ QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/totalreqall-"
                                                                                    "saves.json" };
    if (file.open(QIODevice::ReadWrite))
    {
        QJsonObject item;
        item.insert("type", "custom");
        item.insert("title", m_title->text());
        item.insert("content", m_content->toPlainText());
        item.insert("completed", false);

        QJsonDocument doc{ QJsonDocument::fromJson(file.readAll()) };
        QJsonArray a;

        if (doc.isArray())
            a = doc.array();

        a.append(item);
        doc.setArray(a);
        file.resize(0);
        QTextStream s(&file);
        s << doc.toJson();
        file.close();
    }
    else
        qDebug() << "File couldn't be opened";
}
