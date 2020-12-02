// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com
// SPDX-License-Identifier: BSD-2-Clause

#include "WelcomePage.h"

#include <KLocalizedString>
#include <QCommandLinkButton>
#include <QLabel>
#include <QSettings>
#include <QVBoxLayout>

WelcomePage::WelcomePage(QWidget *parent) : QWidget(parent)
{
    auto layout = new QVBoxLayout;

    m_saved = new QCommandLinkButton{ ki18n("Saved").toString(),
                                      ki18n("Review content that you've saved.").toString() };
    auto bible = new QCommandLinkButton{ ki18n("Bible verse").toString(), ki18n("Memorize a verse "
                                                                                "or verses from "
                                                                                "the Bible.")
                                                                              .toString() };
    auto custom = new QCommandLinkButton{ ki18n("Custom").toString(), ki18n("Enter some custom "
                                                                            "content to "
                                                                            "memorize.")
                                                                          .toString() };

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

    m_saved->setEnabled(hasContent);

    connect(m_saved, &QCommandLinkButton::clicked, this, &WelcomePage::savedClicked);
    connect(bible, &QCommandLinkButton::clicked, this, &WelcomePage::bibleClicked);
    connect(custom, &QCommandLinkButton::clicked, this, &WelcomePage::customClicked);

    layout->addWidget(new QLabel{ ki18n("Choose what to memorize.").toString() });
    layout->addWidget(m_saved);
    layout->addWidget(bible);
    layout->addWidget(custom);

    this->setLayout(layout);
}

void WelcomePage::refresh()
{
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

    m_saved->setEnabled(hasContent);
}
