// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com
// SPDX-License-Identifier: BSD-2-Clause

#ifndef SIMPLEREFCHOOSER_H
#define SIMPLEREFCHOOSER_H

#include <QComboBox>
#include <QDialog>
#include <QGridLayout>
#include <QObject>
#include <QPushButton>

// This class is like the ChooseReferenceWidget, but provides just the bare-bones widget
// with the combos for the reference selection.
class SimpleRefChooser : public QDialog
{
    Q_OBJECT
public:
    SimpleRefChooser(QWidget *, const QString &, const QString &, const QString &, const QString &);

    inline QString getBook()
    {
        return m_book;
    }
    inline QString getChapter()
    {
        return m_chapter;
    }
    inline QString getStartVerse()
    {
        return m_startVerse;
    }
    inline QString getEndVerse()
    {
        return m_endVerse;
    }

private slots:
    void updateChapterVerseValues();
    void updateVerseValues();
    void updateEndVerseValues();
    void updateSaveVerse();

private:
    QString m_book;
    QString m_chapter;
    QString m_startVerse;
    QString m_endVerse;

    QGridLayout *m_layout;
    QComboBox *m_books;
    QComboBox *m_chapters;
    QComboBox *m_startVerses;
    QComboBox *m_endVerses;
    QPushButton *m_ok;
    QPushButton *m_cancel;
};

#endif // SIMPLEREFCHOOSER_H
