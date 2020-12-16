// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-License-Identifier: BSD-2-Clause

#include "SavedContentLoader.h"

#include <KLocalizedString>
#include <QAction>
#include <QDialogButtonBox>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QKeySequence>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QStandardPaths>
#include <QTextStream>
#include <QVBoxLayout>
#include <QXmlStreamReader>
#include <markupfiltmgr.h>
#include <swmgr.h>
#include <swmodule.h>
#include <versekey.h>

SavedContentLoader::SavedContentLoader(QWidget *parent)
    : QWidget(parent),
#ifndef NO_TTS
      m_speak{ new QPushButton{ QIcon::fromTheme("media-playback-start"), i18n("Speak") } },
      m_speaker{ new QTextToSpeech{ QTextToSpeech::availableEngines().first() } },
#endif
      m_contentList{ new QListWidget },
      m_deleteBtn{ new QPushButton{ QIcon::fromTheme("edit-delete"), i18n("Delete") } }
{
    QSettings settings;
    settings.beginGroup("savedContent");
    settings.beginGroup("verses");

    for (auto item : settings.childKeys())
        new QListWidgetItem{ item, m_contentList };

    settings.endGroup();
    settings.beginGroup("custom");

    for (auto item : settings.childKeys())
        new QListWidgetItem{ item, m_contentList };

    settings.endGroup();
    settings.endGroup();

    auto review = new QPushButton{ QIcon::fromTheme("go-next"), i18n("Review") };
    auto back = new QPushButton{ QIcon::fromTheme("go-previous"), i18n("Back") };

    m_contentList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_contentList, &QListWidget::customContextMenuRequested, this,
            [this](const QPoint &pos) {
                QPoint globalPos = m_contentList->mapToGlobal(pos);

                auto rightClickContext = new QMenu;
                auto reviewThisItem = new QAction{ i18n("Review") };
                auto deleteThisItem = new QAction{ i18n("Delete") };
                rightClickContext->addAction(reviewThisItem);
                rightClickContext->addAction(deleteThisItem);

                auto action = rightClickContext->exec(globalPos);
                if (action == reviewThisItem)
                    prepareContent(m_contentList->itemAt(pos));
                else if (action == deleteThisItem)
                    deleteItem(m_contentList->itemAt(pos));
            });

    // SIGNAL/SLOT macros required due to incompatible function signatures
    connect(m_contentList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this,
            SLOT(prepareContent(QListWidgetItem *)));

    connect(review, &QPushButton::clicked, this, [this]() {
        prepareContent(m_contentList->currentItem());
    });
    connect(m_deleteBtn, &QPushButton::clicked, this, [this]() {
        for (auto item : m_contentList->selectedItems())
            deleteItem(item);
    });
    connect(back, &QPushButton::clicked, this, &SavedContentLoader::cancel);

#ifndef NO_TTS
    // TODO: make this work
    m_speak->grabShortcut(QKeySequence{ int{ Qt::Key_MediaTogglePlayPause } });

    m_speak->setDisabled(m_speaker->state() == QTextToSpeech::BackendError);

    connect(m_speaker, &QTextToSpeech::stateChanged, this, [this](QTextToSpeech::State s) {
        if (s == QTextToSpeech::Ready)
            m_speak->setIcon(QIcon::fromTheme("media-playback-start"));
        else if (s == QTextToSpeech::Speaking)
            m_speak->setIcon(QIcon::fromTheme("media-playback-stop"));
    });

    connect(m_speak, &QPushButton::clicked, this, [this]() {
        if (m_speaker->state() == QTextToSpeech::Ready &&
            m_contentList->selectedItems().count() > 0)
        {
            auto item = this->m_contentList->selectedItems().first();
            QString text = prepareContent(item, false);
            if (!text.isNull())
                m_speaker->say(text);
        }
        else if (m_speaker->state() == QTextToSpeech::Speaking)
            m_speaker->stop();
    });
#endif

    auto buttons = new QHBoxLayout;
    buttons->addWidget(back);
    buttons->addStretch();
#ifndef NO_TTS
    buttons->addWidget(m_speak);
#endif
    buttons->addWidget(m_deleteBtn);
    buttons->addWidget(review);

    auto layout = new QVBoxLayout;
    layout->addWidget(new QLabel{ i18n("Double-click an entry to review it.") });
    layout->addWidget(m_contentList);
    layout->addLayout(buttons);

    setLayout(layout);

    refresh();
}

void SavedContentLoader::refresh()
{
    m_contentList->clear();

    QFile file{ QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/totalreqall-"
                                                                                    "saves.json" };
    if (file.open(QIODevice::ReadOnly))
    {
        QJsonDocument doc{ QJsonDocument::fromJson(file.readAll()) };
        if (doc.isArray())
            for (auto item : doc.array())
                if (item.toObject().contains("title"))
                    new QListWidgetItem{ item.toObject()["title"].toString(), m_contentList };

        file.close();
    }

    // load legacy items
    // at some point the QSettings stuff will be entirely removed since it's now deprecated and
    // read-only
    QSettings settings;
    settings.beginGroup("savedContent");
    settings.beginGroup("verses");

    for (auto item : settings.childKeys())
        new QListWidgetItem{ item, m_contentList };

    settings.endGroup();
    settings.beginGroup("custom");

    for (auto item : settings.childKeys())
        new QListWidgetItem{ item, m_contentList };

    settings.endGroup();
    settings.endGroup();
}

QString SavedContentLoader::prepareContent(QListWidgetItem *item)
{
    // emit the signal by default
    return prepareContent(item, true);
}

QString SavedContentLoader::prepareContent(QListWidgetItem *item, bool emitSignal)
{
    bool foundInJson = false; // this is used to determine whether to search in the settings for a
                              // legacy item
    QString content;

    QFile file{ QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/totalreqall-"
                                                                                    "saves.json" };
    if (file.open(QIODevice::ReadOnly))
    {
        QJsonDocument doc{ QJsonDocument::fromJson(file.readAll()) };
        if (doc.isArray())
        {
            for (auto jsonItem : doc.array())
            {
                auto jsonObj = jsonItem.toObject();
                if (jsonObj.contains("title") && jsonObj["title"].toString() == item->text())
                {
                    if (jsonObj.contains("type") && jsonObj["type"].toString() == "verse" &&
                        jsonObj.contains("startRef") && jsonObj.contains("endRef"))
                    {
                        jsonObj["startRef"].toString();
                        sword::SWMgr mgr{ new sword::MarkupFilterMgr{ sword::FMT_PLAIN } };
                        sword::SWModule *module =
                            mgr.getModule(jsonObj["bibleVersion"].toString().toStdString().c_str());
                        sword::VerseKey *key{ static_cast<sword::VerseKey *>(module->getKey()) };
                        key->setText(jsonObj["startRef"].toString().toStdString().c_str());
                        QString text;
                        for (sword::VerseKey otherKey{
                                 jsonObj["endRef"].toString().toStdString().c_str() };
                             *key <= otherKey; key->increment())
                            text += module->renderText();
                        content = text;

                        foundInJson = true;
                    }
                    else if (jsonObj.contains("type") && jsonObj["type"].toString() == "custom" &&
                             jsonObj.contains("content"))
                    {
                        content = jsonObj["content"].toString();
                        foundInJson = true;
                    }
                }
            }
        }

        file.close();
    }

    if (!foundInJson)
    {
        QSettings settings;
        settings.beginGroup("savedContent");

        if (settings.contains("verses/" + item->text()))
        {
            settings.beginGroup("verses");
            sword::SWMgr mgr{ new sword::MarkupFilterMgr{ sword::FMT_PLAIN } };
            sword::SWModule *module = mgr.getModule("KJV");
            sword::VerseKey *key{ static_cast<sword::VerseKey *>(module->getKey()) };
            key->setText(item->text().toStdString().c_str());
            QString text;
            for (sword::VerseKey otherKey{
                     settings.value(item->text()).toString().toStdString().c_str() };
                 *key <= otherKey; key->increment())
                text += module->renderText();
            settings.endGroup(); // verses
            content = text;
        }
        else if (settings.contains("custom/" + item->text()))
            content = settings.value("custom/" + item->text()).toString();
        else
            QMessageBox::critical(this, i18n("Error"),
                                  i18n("Error: could not find data for ") + item->text());

        settings.endGroup(); // savedContent
    }

    if (emitSignal && !content.isNull())
        emit contentReady(content);

    return content;
}

void SavedContentLoader::deleteItem(QListWidgetItem *itemToDelete)
{
    bool foundInJson = false; // this is used to determine whether to search in the settings for a
                              // legacy item

    QFile file{ QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/totalreqall-"
                                                                                    "saves.json" };
    if (file.open(QIODevice::ReadWrite))
    {
        QJsonDocument doc{ QJsonDocument::fromJson(file.readAll()) };
        if (doc.isArray())
        {
            for (auto jsonItem : doc.array())
            {
                auto jsonObj = jsonItem.toObject();
                if (jsonObj.contains("title") &&
                    jsonObj["title"].toString() == itemToDelete->text())
                {
                    auto arr = doc.array();
                    for (int i = 0; i < arr.size(); ++i)
                    {
                        if (arr.at(i).toObject() == jsonObj)
                        {
                            arr.removeAt(i);
                            foundInJson = true;
                            doc.setArray(arr);
                            file.resize(0);
                            QTextStream s(&file);
                            s << doc.toJson();
                            break;
                        }
                    }
                }
            }
        }

        file.close();
    }

    if (!foundInJson)
    {
        QSettings settings;
        settings.beginGroup("savedContent");
        settings.remove("verses/" + itemToDelete->text());
        settings.remove("custom/" + itemToDelete->text());
        settings.endGroup();
    }

    m_contentList->removeItemWidget(itemToDelete);
    refresh();

    if (m_contentList->count() == 0)
        m_deleteBtn->setDisabled(true);
}
