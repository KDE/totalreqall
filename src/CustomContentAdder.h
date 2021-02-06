// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com
// SPDX-License-Identifier: BSD-2-Clause

#ifndef CUSTOMCONTENTADDER_H
#define CUSTOMCONTENTADDER_H

#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>

#ifndef NO_TTS
    #include <QTextToSpeech>
#endif

class CustomContentAdder : public QWidget
{
    Q_OBJECT
public:
    explicit CustomContentAdder(QWidget *parent = nullptr);

    QString getTitle();
    QString getContent();

signals:
    void ok(const QString &);
    void cancel();

private slots:
    void saveItem();

private:
#ifndef NO_TTS
    QPushButton *m_speak;
    QTextToSpeech *m_speaker;
#endif
    QLineEdit *m_title;
    QTextEdit *m_content;
};

#endif // CUSTOMCONTENTADDER_H
