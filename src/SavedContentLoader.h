// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com>>
// SPDX-License-Identifier: BSD-2-Clause

#ifndef SAVEDCONTENTLOADER_H
#define SAVEDCONTENTLOADER_H

#include <QListWidget>
#include <QPushButton>
#include <QWidget>

#ifndef NO_TTS
    #include <QTextToSpeech>
#endif

class SavedContentLoader : public QWidget
{
    Q_OBJECT
public:
    explicit SavedContentLoader(QWidget *parent = nullptr);

signals:
    void contentReady(QString);
    void cancel();

public slots:
    void refresh();

private slots:
    void deleteItem(QListWidgetItem *);
    QString prepareContent(QListWidgetItem *);
    QString prepareContent(QListWidgetItem *, bool);

private:
#ifndef NO_TTS
    QPushButton *m_speak;
    QTextToSpeech *m_speaker;
#endif

    QListWidget *m_contentList;
    QPushButton *m_deleteBtn;
};

#endif // SAVEDCONTENTLOADER_H
