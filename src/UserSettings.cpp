// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com
// SPDX-License-Identifier: BSD-2-Clause

#include "UserSettings.h"

#include <QSettings>
#include <QSharedPointer>

QSharedPointer<UserSettings> UserSettings::s_settings{ new UserSettings };

UserSettings::UserSettings(QObject *parent) : QObject(parent)
{
    refresh();
}

UserSettings::~UserSettings()
{
    save();
}

QSharedPointer<UserSettings> UserSettings::global()
{
    return s_settings;
}

void UserSettings::refresh()
{
    QSettings settings;
    settings.beginGroup("ChooseReferenceWidget");

    m_bibleVersionLoadOption = static_cast<BibleVersionLoadOption>(
        settings.value("bibleVersionLoadOption", BibleVersionLoadOption::LastVersion).toInt());
    m_verseLoadOption = static_cast<VerseLoadOption>(
        settings.value("verseLoadOption", VerseLoadOption::LastVerse).toInt());

    m_lastBook = settings.value("lastBook").toString();
    m_lastChapter = settings.value("lastChapter").toString();
    m_lastStartVerse = settings.value("lastStartVerse").toString();
    m_lastEndVerse = settings.value("lastEndVerse").toString();
    m_defaultBook = settings.value("defaultBook").toString();
    m_defaultChapter = settings.value("defaultChapter").toString();
    m_defaultStartVerse = settings.value("defaultStartVerse").toString();
    m_defaultEndVerse = settings.value("defaultEndVerse").toString();

    m_lastBibleVersion = settings.value("lastBibleVersion").toString();
    m_defaultBibleVersion = settings.value("defaultBibleVersion").toString();

    settings.endGroup(); // ChooseReferenceWidget
    settings.beginGroup("MainWindow");

    m_windowWidth = settings.value("width").toInt();
    m_windowHeight = settings.value("height").toInt();
    m_saveWinSize = settings.value("saveWinSize", true).toBool();

    settings.endGroup(); // MainWindow
    settings.beginGroup("MemorizeEdit");

    m_memorizeErrorAction =
        static_cast<ErrorAction>(settings.value("errorAction", ErrorAction::Redo).toInt());
    m_splitContent = settings.value("splitContent", true).toBool();
    m_splitThreshold = settings.value("splitThreshold", 200).toInt();
    m_chunkSize = settings.value("chunkSize", 100).toInt();

    settings.endGroup(); // MemorizeEdit
}

void UserSettings::save()
{
    QSettings settings;
    settings.beginGroup("ChooseReferenceWidget");

    settings.setValue("bibleVersionLoadOption", static_cast<int>(m_bibleVersionLoadOption));
    settings.setValue("verseLoadOption", static_cast<int>(m_verseLoadOption));

    settings.setValue("lastBook", m_lastBook);
    settings.setValue("lastChapter", m_lastChapter);
    settings.setValue("lastStartVerse", m_lastStartVerse);
    settings.setValue("lastEndVerse", m_lastEndVerse);
    settings.setValue("defaultBook", m_defaultBook);
    settings.setValue("defaultChapter", m_defaultChapter);
    settings.setValue("defaultStartVerse", m_defaultStartVerse);
    settings.setValue("defaultEndVerse", m_defaultEndVerse);

    settings.endGroup(); // ChooseReferenceWidget
    settings.beginGroup("MainWindow");

    settings.setValue("width", m_windowWidth);
    settings.setValue("height", m_windowHeight);
    settings.setValue("saveWinSize", m_saveWinSize);

    settings.endGroup(); // MainWindow
    settings.beginGroup("MemorizeEdit");

    settings.setValue("errorAction", m_memorizeErrorAction);
    settings.setValue("splitContent", m_splitContent);
    settings.setValue("splitThreshold", m_splitThreshold);
    settings.setValue("chunkSize", m_chunkSize);

    settings.endGroup(); // MemorizeEdit
}

void UserSettings::setBibleVersionLoadOption(BibleVersionLoadOption option)
{
    if (option != m_bibleVersionLoadOption)
    {
        m_bibleVersionLoadOption = option;
        save();
        emit bibleVersionLoadOptionChanged(m_bibleVersionLoadOption);
    }
}

void UserSettings::setVerseLoadOption(VerseLoadOption option)
{
    if (option != m_verseLoadOption)
    {
        m_verseLoadOption = option;
        save();
        emit verseLoadOptionChanged(m_verseLoadOption);
    }
}

void UserSettings::setMemorizeErrorAction(ErrorAction action)
{
    if (action != m_memorizeErrorAction)
    {
        m_memorizeErrorAction = action;
        save();
        emit memorizeErrorActionChanged(m_memorizeErrorAction);
    }
}

void UserSettings::setLastBook(QString book)
{
    if (book != m_lastBook)
    {
        m_lastBook = book;
        save();
        emit lastBookChanged(m_lastBook);
    }
}

void UserSettings::setLastChapter(QString chapter)
{
    if (chapter != m_lastChapter)
    {
        m_lastChapter = chapter;
        save();
        emit lastChapterChanged(m_lastChapter);
    }
}

void UserSettings::setLastStartVerse(QString verse)
{
    if (verse != m_lastStartVerse)
    {
        m_lastStartVerse = verse;
        save();
        emit lastStartVerseChanged(m_lastStartVerse);
    }
}

void UserSettings::setLastEndVerse(QString verse)
{
    if (verse != m_lastEndVerse)
    {
        m_lastEndVerse = verse;
        save();
        emit lastEndVerseChanged(m_lastEndVerse);
    }
}

void UserSettings::setDefaultBook(QString book)
{
    if (book != m_defaultBook)
    {
        m_defaultBook = book;
        save();
        emit defaultBookChanged(m_defaultBook);
    }
}

void UserSettings::setDefaultChapter(QString chapter)
{
    if (chapter != m_defaultChapter)
    {
        m_defaultChapter = chapter;
        save();
        emit defaultChapterChanged(m_defaultChapter);
    }
}

void UserSettings::setDefaultStartVerse(QString verse)
{
    if (verse != m_defaultStartVerse)
    {
        m_defaultStartVerse = verse;
        save();
        emit defaultStartVerseChanged(m_defaultStartVerse);
    }
}

void UserSettings::setDefaultEndVerse(QString verse)
{
    if (verse != m_defaultEndVerse)
    {
        m_defaultEndVerse = verse;
        save();
        emit defaultEndVerseChanged(m_defaultEndVerse);
    }
}

void UserSettings::setLastBibleVersion(QString version)
{
    if (version != m_lastBibleVersion)
    {
        m_lastBibleVersion = version;
        save();
        emit lastEndVerseChanged(m_lastBibleVersion);
    }
}

void UserSettings::setDefaultBibleVersion(QString version)
{
    if (version != m_defaultBibleVersion)
    {
        m_defaultBibleVersion = version;
        save();
        emit lastEndVerseChanged(m_defaultBibleVersion);
    }
}

void UserSettings::setWindowWidth(int width)
{
    if (width != m_windowWidth)
    {
        m_windowWidth = width;
        save();
        emit windowWidthChanged(m_windowWidth);
    }
}

void UserSettings::setWindowHeight(int height)
{
    if (height != m_windowHeight)
    {
        m_windowHeight = height;
        save();
        emit windowHeightChanged(m_windowHeight);
    }
}

void UserSettings::setSaveWinSize(bool b)
{
    if (b != m_saveWinSize)
    {
        m_saveWinSize = b;
        save();
        emit saveWinSizeChanged(m_saveWinSize);
    }
}

void UserSettings::setSplitContent(bool b)
{
    if (b != m_splitContent)
    {
        m_splitContent = b;
        save();
        emit splitContentChanged(m_splitContent);
    }
}

void UserSettings::setSplitThreshold(int threshold)
{
    if (threshold != m_splitThreshold)
    {
        m_splitThreshold = threshold;
        save();
        emit splitThresholdChanged(m_splitThreshold);
    }
}

void UserSettings::setChunkSize(int size)
{
    if (size != m_chunkSize)
    {
        m_chunkSize = size;
        save();
        emit chunkSizeChanged(m_chunkSize);
    }
}

BibleVersionLoadOption UserSettings::bibleVersionLoadOption() const
{
    return m_bibleVersionLoadOption;
}

VerseLoadOption UserSettings::verseLoadOption() const
{
    return m_verseLoadOption;
}

ErrorAction UserSettings::memorizeErrorAction() const
{
    return m_memorizeErrorAction;
}

QString UserSettings::lastBook() const
{
    return m_lastBook;
}
QString UserSettings::lastChapter() const
{
    return m_lastChapter;
}
QString UserSettings::lastStartVerse() const
{
    return m_lastStartVerse;
}
QString UserSettings::lastEndVerse() const
{
    return m_lastEndVerse;
}
QString UserSettings::defaultBook() const
{
    return m_defaultBook;
}
QString UserSettings::defaultChapter() const
{
    return m_defaultChapter;
}
QString UserSettings::defaultStartVerse() const
{
    return m_defaultStartVerse;
}
QString UserSettings::defaultEndVerse() const
{
    return m_defaultEndVerse;
}

QString UserSettings::lastBibleVersion() const
{
    return m_lastBibleVersion;
}
QString UserSettings::defaultBibleVersion() const
{
    return m_defaultBibleVersion;
}

int UserSettings::windowWidth() const
{
    return m_windowWidth;
}

int UserSettings::windowHeight() const
{
    return m_windowHeight;
}

bool UserSettings::saveWinSize() const
{
    return m_saveWinSize;
}

bool UserSettings::splitContent() const
{
    return m_splitContent;
}

int UserSettings::splitThreshold() const
{
    return m_splitThreshold;
}

int UserSettings::chunkSize() const
{
    return m_chunkSize;
}
