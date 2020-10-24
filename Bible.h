#ifndef BIBLE_H
#define BIBLE_H

#include <string>

#include <QObject>
#include <QMap>
#include <QStringList>
#include <QFile>

class Bible : public QObject
{
    Q_OBJECT
public:
    explicit Bible(QObject *parent = nullptr);

    QMap<QString, int> m_chaptersPerBook;
    QMap<int, int> m_versesPerChapter;

signals:

private:
    QStringList m_booksOfBible;
    QFile *m_bibleFile;
    std::string m_bibleData;

    int scrapeChaptersPerBook(const QString &);
    int scrapeVersesPerChapter(const QString &, const int);
};

#endif // BIBLE_H
