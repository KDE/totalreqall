#ifndef MEMORIZEEDIT_H
#define MEMORIZEEDIT_H

#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QKeyEvent>

enum ErrorAction
{
	Redo = 1,
	KeepGoing = 2,
};

class MemorizeEdit : public QTextEdit
{
	Q_OBJECT
public:
	MemorizeEdit(QString &, QWidget * = nullptr);

signals:
	void done();
	void messageToUser(QString message);

protected:
	virtual void keyPressEvent(QKeyEvent *);

private:
	QStringList m_words;
	QString m_richText;

	ErrorAction m_errorAction;
};

#endif // MEMORIZEEDIT_H
