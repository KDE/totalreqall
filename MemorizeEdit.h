#ifndef MEMORIZEEDIT_H
#define MEMORIZEEDIT_H

#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QKeyEvent>

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
};

#endif // MEMORIZEEDIT_H
