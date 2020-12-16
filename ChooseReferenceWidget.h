// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-License-Identifier: BSD-2-Clause

#ifndef CHOOSEREFERENCEWIDGET_H
#define CHOOSEREFERENCEWIDGET_H

#include <QComboBox>
#include <QObject>
#include <QPushButton>
#include <QTextBrowser>
#include <QTextEdit>
#include <QWidget>

#include <map>

#ifndef NO_TTS
#include <QTextToSpeech>
#endif

enum VerseLoadOption : int
{
    LastVerse = 1,
    RandomVerse = 2,
    SetVerse = 3,
};

enum BibleVersionLoadOption : int
{
    LastVersion = 1,
    RandomVersion = 2,
    SetVersion = 3,
};

class ChooseReferenceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChooseReferenceWidget(QWidget *parent = nullptr);

    bool unusable();

signals:
    void startMemorizer(const QString &);
    void cancel();

private slots:
    void updateChapterVerseValues();
    void updateVerseValues();
    void updateEndVerseValues();
    void updateSaveVerse();
    void saveItem();

    void displayVerse();
    void runMemorizer();

private:
#ifndef NO_TTS
    QTextToSpeech *m_speaker;
    QPushButton *m_speak;
#endif

    QComboBox *m_books;
    QComboBox *m_chapters;
    QComboBox *m_startVerses;
    QComboBox *m_endVerses;
    QComboBox *m_bibleVersion;
    QTextBrowser *m_verseDisplayBox;

    QString m_currentBibleVersion;

    bool m_unusable = false;
};

#endif // CHOOSEREFERENCEWIDGET_H
