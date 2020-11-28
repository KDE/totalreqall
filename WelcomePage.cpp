// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com
// SPDX-License-Identifier: BSD-2-Clause

#include "WelcomePage.h"

#include <QCommandLinkButton>
#include <QLabel>
#include <QSettings>
#include <QVBoxLayout>

WelcomePage::WelcomePage(QWidget *parent) : QWidget(parent)
{
    auto layout = new QVBoxLayout;

    auto saved = new QCommandLinkButton{ tr("Saved"), tr("Review content that you've saved.") };
    auto bible = new QCommandLinkButton{ tr("Bible verse"), tr("Memorize a verse or verses from "
                                                               "the Bible.") };
    auto custom = new QCommandLinkButton{ tr("Custom"), tr("Enter some custom content to "
                                                           "memorize.") };

    bool hasContent = false;
    QSettings settings;
    settings.beginGroup("savedContent");

    settings.beginGroup("verses");
    hasContent |= !settings.childKeys().isEmpty();
    settings.endGroup(); // verses

    settings.beginGroup("custom");
    hasContent |= !settings.childKeys().isEmpty();
    settings.endGroup(); // custom

    settings.endGroup(); // savedContent

    saved->setEnabled(hasContent);

    connect(saved, &QCommandLinkButton::clicked, this, &WelcomePage::savedClicked);
    connect(bible, &QCommandLinkButton::clicked, this, &WelcomePage::bibleClicked);
    connect(custom, &QCommandLinkButton::clicked, this, &WelcomePage::customClicked);

    layout->addWidget(new QLabel{ tr("Choose what to memorize.") });
    layout->addWidget(saved);
    layout->addWidget(bible);
    layout->addWidget(custom);

    this->setLayout(layout);
}
