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

enum class VerseLoadOption : int
{
    Last = 1,
    Random = 2,
    Set = 3,
};

// operator int(VerseLoadOption &op)
//{
//	return static_cast<int>(op);
//}

// operator VerseLoadOption(int &i)
//{
//	return static_cast<VerseLoadOption>(i);
//}

enum class BibleVersionLoadOption : int
{
    Last = 1,
    Random = 2,
    Set = 3,
};

// operator int(BibleVersionLoadOption &op)
//{
//	return static_cast<int>(op);
//}

// operator BibleVersionLoadOption(int &i)
//{
//	return static_cast<BibleVersionLoadOption>(i);
//}

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
