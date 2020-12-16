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
        settings.value("bibleVersionLoadOption", static_cast<int>(BibleVersionLoadOption::Last))
            .toInt());
    m_verseLoadOption = static_cast<VerseLoadOption>(
        settings.value("verseLoadOption", static_cast<int>(VerseLoadOption::Last)).toInt());

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

    settings.endGroup(); // MemorizeEdit
}

void UserSettings::setBibleVersionLoadOption(BibleVersionLoadOption option)
{
    if (option != m_bibleVersionLoadOption)
    {
        m_bibleVersionLoadOption = option;
        QSettings settings;
        settings.setValue("ChooseReferenceWidget/bibleVersionLoadOption",
                          static_cast<int>(m_bibleVersionLoadOption));
        emit bibleVersionLoadOptionChanged(m_bibleVersionLoadOption);
    }
}

void UserSettings::setVerseLoadOption(VerseLoadOption option)
{
    if (option != m_verseLoadOption)
    {
        m_verseLoadOption = option;
        QSettings settings;
        settings.setValue("ChooseReferenceWidget/verseLoadOption",
                          static_cast<int>(m_verseLoadOption));
        emit verseLoadOptionChanged(m_verseLoadOption);
    }
}

void UserSettings::setMemorizeErrorAction(ErrorAction action)
{
    if (action != m_memorizeErrorAction)
    {
        m_memorizeErrorAction = action;
        QSettings settings;
        settings.setValue("MemorizeEdit/errorAction", m_memorizeErrorAction);
        emit memorizeErrorActionChanged(m_memorizeErrorAction);
    }
}

void UserSettings::setLastBook(QString book)
{
    if (book != m_lastBook)
    {
        m_lastBook = book;
        QSettings settings;
        settings.setValue("ChooseReferenceWidget/lastBook", m_lastBook);
        emit lastBookChanged(m_lastBook);
    }
}

void UserSettings::setLastChapter(QString chapter)
{
    if (chapter != m_lastChapter)
    {
        m_lastChapter = chapter;
        QSettings settings;
        settings.setValue("ChooseReferenceWidget/lastChapter", m_lastChapter);
        emit lastChapterChanged(m_lastChapter);
    }
}

void UserSettings::setLastStartVerse(QString verse)
{
    if (verse != m_lastStartVerse)
    {
        m_lastStartVerse = verse;
        QSettings settings;
        settings.setValue("ChooseReferenceWidget/lastStartVerse", m_lastStartVerse);
        emit lastStartVerseChanged(m_lastStartVerse);
    }
}

void UserSettings::setLastEndVerse(QString verse)
{
    if (verse != m_lastEndVerse)
    {
        m_lastEndVerse = verse;
        QSettings settings;
        settings.setValue("ChooseReferenceWidget/lastBook", m_lastEndVerse);
        emit lastEndVerseChanged(m_lastEndVerse);
    }
}

void UserSettings::setDefaultBook(QString book)
{
    if (book != m_defaultBook)
    {
        m_defaultBook = book;
        QSettings settings;
        settings.setValue("ChooseReferenceWidget/defaultBook", m_defaultBook);
        emit defaultBookChanged(m_defaultBook);
    }
}

void UserSettings::setDefaultChapter(QString chapter)
{
    if (chapter != m_defaultChapter)
    {
        m_defaultChapter = chapter;
        QSettings settings;
        settings.setValue("ChooseReferenceWidget/defaultChapter", m_defaultChapter);
        emit defaultChapterChanged(m_defaultChapter);
    }
}

void UserSettings::setDefaultStartVerse(QString verse)
{
    if (verse != m_defaultStartVerse)
    {
        m_defaultStartVerse = verse;
        QSettings settings;
        settings.setValue("ChooseReferenceWidget/defaultStartVerse", m_defaultStartVerse);
        emit defaultStartVerseChanged(m_defaultStartVerse);
    }
}

void UserSettings::setDefaultEndVerse(QString verse)
{
    if (verse != m_defaultEndVerse)
    {
        m_defaultEndVerse = verse;
        QSettings settings;
        settings.setValue("ChooseReferenceWidget/defaultEndVerse", m_defaultEndVerse);
        emit defaultEndVerseChanged(m_defaultEndVerse);
    }
}

void UserSettings::setLastBibleVersion(QString version)
{
    if (version != m_lastBibleVersion)
    {
        m_lastBibleVersion = version;
        QSettings settings;
        settings.setValue("ChooseReferenceWidget/lastBibleVersion", m_lastBibleVersion);
        emit lastEndVerseChanged(m_lastBibleVersion);
    }
}

void UserSettings::setDefaultBibleVersion(QString version)
{
    if (version != m_defaultBibleVersion)
    {
        m_defaultBibleVersion = version;
        QSettings settings;
        settings.setValue("ChooseReferenceWidget/defaultBibleVersion", m_defaultBibleVersion);
        emit lastEndVerseChanged(m_defaultBibleVersion);
    }
}

void UserSettings::setWindowWidth(int width)
{
    if (width != m_windowWidth)
    {
        m_windowWidth = width;
        QSettings settings;
        settings.setValue("MainWindow/width", m_windowWidth);
        emit windowWidthChanged(m_windowWidth);
    }
}

void UserSettings::setWindowHeight(int height)
{
    if (height != m_windowHeight)
    {
        m_windowHeight = height;
        QSettings settings;
        settings.setValue("MainWindow/height", m_windowHeight);
        emit windowHeightChanged(m_windowHeight);
    }
}

void UserSettings::setSaveWinSize(bool b)
{
    if (b != m_saveWinSize)
    {
        m_saveWinSize = b;
        QSettings settings;
        settings.setValue("MainWindow/saveWinSize", m_saveWinSize);
        emit saveWinSizeChanged(m_saveWinSize);
    }
}

void UserSettings::setSplitContent(bool b)
{
    if (b != m_splitContent)
    {
        m_splitContent = b;
        QSettings settings;
        settings.setValue("MemorizeEdit/splitContent", m_splitContent);
        emit splitContentChanged(m_splitContent);
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
