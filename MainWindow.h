// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-License-Identifier: BSD-2-Clause

#ifndef MainWindow_H
#define MainWindow_H

#include "ChooseReferenceWidget.h"
#include "CustomContentAdder.h"
#include "MemorizeWidget.h"
#include "SavedContentLoader.h"
#include "WelcomePage.h"
#include <KMainWindow>
#include <QAction>
#include <QGridLayout>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QWidget>

class MainWindow : public KMainWindow
{
    Q_OBJECT

public:
	MainWindow(KMainWindow *parent = nullptr);
    ~MainWindow();

protected:
    virtual void resizeEvent(QResizeEvent *);

private:
    ChooseReferenceWidget *m_refChooser;
    WelcomePage *m_welcomePage;
    CustomContentAdder *m_contentAdder;
    SavedContentLoader *m_savedLoader;

    // when a central widget needs removed but not deleted
    QWidget *m_saveCentralWidget = nullptr;
    QWidget *m_saveFocusWidget = nullptr;
    MemorizeWidget *m_memorizer = nullptr;

private slots:
    void runMemorizer(const QString &);
    void cleanUpMemorizer();
    void setStatusMessage(QString);
};
#endif // MainWindow_H
