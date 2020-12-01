// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-License-Identifier: BSD-2-Clause

#ifndef CHOOSEREFERENCEWIDGET_H
#define CHOOSEREFERENCEWIDGET_H

#include <QComboBox>
#include <QGridLayout>
#include <QObject>
#include <QPushButton>
#include <QTextBrowser>
#include <QTextEdit>
#include <QWidget>
#include <map>

enum VerseLoadOption : short
{
    Saved = 1,
    Random = 2,
    Set = 3,
};

class ChooseReferenceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChooseReferenceWidget(QWidget *parent = nullptr);

signals:
    void startMemorizer(const QString &);
    void cancel();

private slots:
    void updateChapterVerseValues();
    void updateVerseValues();
    void updateEndVerseValues();
    void updateSaveVerse();

    void displayVerse();
    void runMemorizer();

private:
    QGridLayout *m_layout;
    QComboBox *m_books;
    QComboBox *m_chapters;
    QComboBox *m_startVerses;
    QComboBox *m_endVerses;
    QPushButton *m_runMemorizerBtn;
    QPushButton *m_displayVerseBtn;
    QTextBrowser *m_verseDisplayBox;
};

#endif // CHOOSEREFERENCEWIDGET_H
