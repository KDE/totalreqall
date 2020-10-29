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

class MemorizeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MemorizeWidget(QString, QWidget * = nullptr);
    ~MemorizeWidget();

protected:
    virtual void keyPressEvent(QKeyEvent *);

signals:
    void done();

private:
    QLabel *m_displayText;
    QStringList m_words;
	QLabel *m_statusLabel;
    QGridLayout *m_layout;

	QTimer *m_endMemorizerTimer = nullptr;
};

#endif // MEMORIZEWIDGET_H
