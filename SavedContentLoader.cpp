// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-License-Identifier: BSD-2-Clause

#include "SavedContentLoader.h"

#include <QAction>
#include <QDialogButtonBox>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>
#include <sword/markupfiltmgr.h>
#include <sword/swmgr.h>
#include <sword/swmodule.h>
#include <sword/versekey.h>

SavedContentLoader::SavedContentLoader(QWidget *parent)
    : QWidget(parent),
      m_contentList{ new QListWidget }
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

    auto review = new QPushButton{ QIcon::fromTheme("go-next"), tr("Review") };
    auto deleteItem = new QPushButton{ QIcon::fromTheme("edit-delete"), tr("Delete") };
    auto back = new QPushButton{ QIcon::fromTheme("go-previous"), tr("Back") };

    auto deleteLambda = [this, deleteItem](QListWidgetItem *itemToDelete) {
        QSettings settings;
        settings.beginGroup("savedContent");
        settings.remove("verses/" + itemToDelete->text());
        settings.remove("custom/" + itemToDelete->text());
        settings.endGroup();
        m_contentList->removeItemWidget(itemToDelete);
        refresh();

        if (m_contentList->count() == 0)
            deleteItem->setDisabled(true);
    };

    m_contentList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_contentList, &QListWidget::customContextMenuRequested, this,
            [this, deleteLambda](const QPoint &pos) {
                QPoint globalPos = m_contentList->mapToGlobal(pos);

                auto rightClickContext = new QMenu;
                auto reviewItem = new QAction{ tr("Review") };
                auto deleteItem = new QAction{ tr("Delete") };
                rightClickContext->addAction(reviewItem);
                rightClickContext->addAction(deleteItem);

                auto action = rightClickContext->exec(globalPos);
                if (action == reviewItem)
                    prepareContent(m_contentList->itemAt(pos));
                else if (action == deleteItem)
                    deleteLambda(m_contentList->itemAt(pos));
            });

    connect(m_contentList, &QListWidget::itemDoubleClicked, this,
            &SavedContentLoader::prepareContent);

    connect(review, &QPushButton::clicked, this, [this]() {
        prepareContent(m_contentList->currentItem());
    });
    connect(deleteItem, &QPushButton::clicked, this, [this, deleteLambda]() {
        for (auto item : m_contentList->selectedItems())
            deleteLambda(item);
    });
    connect(back, &QPushButton::clicked, this, &SavedContentLoader::cancel);

    auto buttons = new QHBoxLayout;
    buttons->addWidget(back);
    buttons->addStretch();
    buttons->addWidget(deleteItem);
    buttons->addWidget(review);

    auto layout = new QVBoxLayout;
    layout->addWidget(new QLabel{ tr("Double-click an entry to review it.") });
    layout->addWidget(m_contentList);
    layout->addLayout(buttons);

    setLayout(layout);
}

void SavedContentLoader::refresh()
{
    m_contentList->clear();

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

void SavedContentLoader::prepareContent(QListWidgetItem *item)
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
        settings.endGroup();
        emit contentReady(text);
    }
    else if (settings.contains("custom/" + item->text()))
        emit contentReady(settings.value("custom/" + item->text()).toString());
    else
        QMessageBox::critical(this, tr("Error"),
                              tr("Error: could not find data for ") + item->text());

    settings.endGroup();
}
