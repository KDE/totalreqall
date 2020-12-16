// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com
// SPDX-License-Identifier: BSD-2-Clause

#ifndef MEMORIZEWIDGET_H
#define MEMORIZEWIDGET_H

#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QSet>
#include <QString>
#include <QStringList>
#include <QTextEdit>
#include <QTimer>
#include <QWidget>

enum ErrorAction : int
{
    Redo = 1,
    KeepGoing = 2,
};

enum Difficulty : int
{
    Easy = 0,
    Medium = 1,
    Hard = 2,
};

class MemorizeEdit : public QTextEdit
{
    Q_OBJECT
public:
    MemorizeEdit(QString &, Difficulty, QWidget * = nullptr);

signals:
    void done();
    void messageToUser(QString message);

protected:
    virtual void keyPressEvent(QKeyEvent *) override;

private:
    QString formattedEndString(Difficulty);

    QStringList m_words;
    int m_numWords;

    QString m_richText;

    ErrorAction m_errorAction;
    Difficulty m_difficulty;
};

class MemorizeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MemorizeWidget(QString, QWidget * = nullptr);
    ~MemorizeWidget();

signals:
    void done();
    void newStatus(QString message);

private slots:
    void editDone();
    void nextLevel();

private:
    QGridLayout *m_layout;
    QPushButton *m_endSession;
    MemorizeEdit *m_memorizeEdit;

    Difficulty m_difficulty;
    QStringList m_content;
    int m_contentIndex = 0;
    QString m_sep;

    QTimer *m_endMemorizerTimer = nullptr;

    static QSet<QString> s_memorizedContent;
};

#endif // MEMORIZEWIDGET_H
