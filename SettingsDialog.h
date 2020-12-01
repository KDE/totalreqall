// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com
// SPDX-License-Identifier: BSD-2-Clause

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QButtonGroup>
#include <QCheckBox>
#include <QDialog>
#include <QPushButton>
#include <QRadioButton>
#include <QTabWidget>

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = nullptr);

signals:

private slots:
    void ok();
    void apply();

private:
    QTabWidget *m_tabs;

    QButtonGroup *m_errorActionSettings;
    QRadioButton *m_redo;
    QRadioButton *m_keepGoing;

#ifndef Q_OS_WASM // nothing in this block is applicable for WASM
    QButtonGroup *m_verseLoadSettings;
    QRadioButton *m_saveVerse;
    QRadioButton *m_randVerse;
    QRadioButton *m_setVerse;
    QPushButton *m_chooseSetVerse;

    QCheckBox *m_shouldSaveWindowSize;
#endif // Q_OS_WASM

    QPushButton *m_reset;
};

#endif // SETTINGSDIALOG_H
