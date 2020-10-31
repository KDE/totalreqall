#ifndef MEMORIZEWIDGET_H
#define MEMORIZEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QKeyEvent>
#include <QStringList>
#include <QGridLayout>
#include <QTimer>
#include <QPushButton>

#include "MemorizeEdit.h"

class MemorizeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MemorizeWidget(QString, QWidget * = nullptr);
	~MemorizeWidget();
	void focusMemorizer() { m_memorizeEdit->setFocus(); }

signals:
	void done();
	void newStatus(QString message);

private slots:
	void editDone();

private:
    QGridLayout *m_layout;
	QPushButton *m_endSession;
	MemorizeEdit *m_memorizeEdit;

	QTimer *m_endMemorizerTimer = nullptr;
};

#endif // MEMORIZEWIDGET_H
