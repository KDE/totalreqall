#ifndef MEMORIZEWIDGET_H
#define MEMORIZEWIDGET_H

#include "MemorizeEdit.h"
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QSet>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QWidget>

class MemorizeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MemorizeWidget(QString, QWidget * = nullptr);
    ~MemorizeWidget();
    void focusMemorizer()
    {
        m_memorizeEdit->setFocus();
    }

signals:
    void done();
    void newStatus(QString message);

private slots:
    void editDone();
    void nextLevel();

private:
    QGridLayout *m_layout;
    QPushButton *m_endSession;
    MemorizeEdit *m_memorizeEdit;

    Difficulty m_difficulty;
    QString m_content;

    QTimer *m_endMemorizerTimer = nullptr;

    static QSet<QString> s_memorizedContent;
};

#endif // MEMORIZEWIDGET_H
