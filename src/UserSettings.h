// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com
// SPDX-License-Identifier: BSD-2-Clause

#ifndef USERSETTINGS_H
#define USERSETTINGS_H

#include "ChooseReferenceWidget.h"
#include "MemorizeWidget.h"
#include <QObject>

class UserSettings : public QObject
{
    Q_OBJECT

public:
    explicit UserSettings(QObject *parent = nullptr);
    ~UserSettings();
    void refresh();
    void save();

    static QSharedPointer<UserSettings> global();

    BibleVersionLoadOption bibleVersionLoadOption() const;
    VerseLoadOption verseLoadOption() const;
    ErrorAction memorizeErrorAction() const;

    QString lastBook() const;
    QString lastChapter() const;
    QString lastStartVerse() const;
    QString lastEndVerse() const;
    QString defaultBook() const;
    QString defaultChapter() const;
    QString defaultStartVerse() const;
    QString defaultEndVerse() const;

    QString lastBibleVersion() const;
    QString defaultBibleVersion() const;

    int windowWidth() const;
    int windowHeight() const;
    bool saveWinSize() const;

    bool splitContent() const;
    int splitThreshold() const;
    int chunkSize() const;

    void setBibleVersionLoadOption(BibleVersionLoadOption);
    void setVerseLoadOption(VerseLoadOption);
    void setMemorizeErrorAction(ErrorAction);

    void setLastBook(QString);
    void setLastChapter(QString);
    void setLastStartVerse(QString);
    void setLastEndVerse(QString);
    void setDefaultBook(QString);
    void setDefaultChapter(QString);
    void setDefaultStartVerse(QString);
    void setDefaultEndVerse(QString);

    void setLastBibleVersion(QString);
    void setDefaultBibleVersion(QString);

    void setWindowWidth(int);
    void setWindowHeight(int);
    void setSaveWinSize(bool);

    void setSplitContent(bool);
    void setSplitThreshold(int);
    void setChunkSize(int);

signals:
    void bibleVersionLoadOptionChanged(BibleVersionLoadOption &);
    void verseLoadOptionChanged(VerseLoadOption &);
    void memorizeErrorActionChanged(ErrorAction &);

    void lastBookChanged(QString &);
    void lastChapterChanged(QString &);
    void lastStartVerseChanged(QString &);
    void lastEndVerseChanged(QString &);
    void defaultBookChanged(QString &);
    void defaultChapterChanged(QString &);
    void defaultStartVerseChanged(QString &);
    void defaultEndVerseChanged(QString &);

    void lastBibleVersionChanged(QString &);
    void defaultBibleVersionChanged(QString &);

    void windowWidthChanged(int &);
    void windowHeightChanged(int &);
    void saveWinSizeChanged(bool &);

    void splitContentChanged(bool &);
    void splitThresholdChanged(int &);
    void chunkSizeChanged(int &);

private:
    BibleVersionLoadOption m_bibleVersionLoadOption;
    VerseLoadOption m_verseLoadOption;
    ErrorAction m_memorizeErrorAction;

    QString m_lastBook;
    QString m_lastChapter;
    QString m_lastStartVerse;
    QString m_lastEndVerse;
    QString m_defaultBook;
    QString m_defaultChapter;
    QString m_defaultStartVerse;
    QString m_defaultEndVerse;

    QString m_lastBibleVersion;
    QString m_defaultBibleVersion;

    int m_windowWidth;
    int m_windowHeight;
    bool m_saveWinSize;

    bool m_splitContent;
    int m_splitThreshold;
    int m_chunkSize;

    static QSharedPointer<UserSettings> s_settings;
};

#endif // USERSETTINGS_H
