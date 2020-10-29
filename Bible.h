#ifndef BIBLE_H
#define BIBLE_H

#include <string>

#include <QObject>
#include <QMap>
#include <QStringList>
#include <QFile>
#include <QTimer>

class Bible : public QObject
{
    Q_OBJECT
public:
    explicit Bible(QObject *parent = nullptr);
	~Bible();

    QMap<QString, int> m_chaptersPerBook;
    QMap<int, int> m_versesPerChapter;

	void readData();
	void freeData();

	int scrapeChaptersPerBook(const QString &);
	int scrapeVersesPerChapter(const QString &, const QString &);
	int scrapeVersesPerChapter(const QString &, const int);

	QString getVerseStringFromRef(const QString &);

signals:

private:
	QStringList m_booksOfBible;
	QFile *m_bibleFile;
	QString m_bibleData;

	QTimer *m_freeTimer;
};

#endif // BIBLE_H
