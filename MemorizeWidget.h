#ifndef MEMORIZEWIDGET_H
#define MEMORIZEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QKeyEvent>
#include <QStringList>
#include <QGridLayout>

class MemorizeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MemorizeWidget(QString, QWidget * = nullptr);
    ~MemorizeWidget();

protected:
    virtual void keyPressEvent(QKeyEvent *);

signals:
    void contentCompleted();

private:
    QLabel *m_displayText;
    QStringList m_words;

    QGridLayout *m_layout;

    QLabel *m_statusLabel;

};

#endif // MEMORIZEWIDGET_H
