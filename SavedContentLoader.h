// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com>>
// SPDX-License-Identifier: BSD-2-Clause

#ifndef SAVEDCONTENTLOADER_H
#define SAVEDCONTENTLOADER_H

#include <QListWidget>
#include <QPushButton>
#include <QWidget>

class SavedContentLoader : public QWidget
{
    Q_OBJECT
public:
    explicit SavedContentLoader(QWidget *parent = nullptr);

signals:
    void contentReady(const QString &);
    void cancel();

public slots:
    void refresh();

private slots:
    void deleteItem(QListWidgetItem *);

private:
    void prepareContent(QListWidgetItem *);

    QListWidget *m_contentList;
    QPushButton *m_deleteBtn;
};

#endif // SAVEDCONTENTLOADER_H
