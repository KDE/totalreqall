// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com
// SPDX-License-Identifier: BSD-2-Clause

#ifndef MEMORIZEEDIT_H
#define MEMORIZEEDIT_H

#include <QKeyEvent>
#include <QObject>
#include <QTextEdit>
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
    virtual void keyPressEvent(QKeyEvent *);

private:
    QString formattedEndString(Difficulty);

    QStringList m_words;
    int m_numWords;

    QString m_richText;

    ErrorAction m_errorAction;
    Difficulty m_difficulty;
};

#endif // MEMORIZEEDIT_H
