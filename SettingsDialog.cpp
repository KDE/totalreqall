// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com
// SPDX-License-Identifier: BSD-2-Clause

#include "SettingsDialog.h"

#include "ChooseReferenceWidget.h"
#include "MemorizeWidget.h"
#include "SimpleRefChooser.h"
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSettings>
#include <QVBoxLayout>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog{ parent },
      m_tabs{ new QTabWidget },
      m_errorActionSettings{ new QButtonGroup },
      m_redo{ new QRadioButton },
      m_keepGoing{ new QRadioButton },
#ifndef Q_OS_WASM // skip the unecessary stuff
      m_verseLoadSettings{ new QButtonGroup },
      m_saveVerse{ new QRadioButton },
      m_randVerse{ new QRadioButton },
      m_setVerse{ new QRadioButton },
      m_chooseSetVerse{ new QPushButton },
      m_shouldSaveWindowSize{ new QCheckBox },
#endif // Q_OS_WASM
      m_reset{ new QPushButton }
{
    this->setWindowTitle(tr("Settings"));

    m_tabs->setUsesScrollButtons(false);

    // get our settings object
    QSettings settings;

    // set up the widgets
    m_redo->setText(tr("&Retype the word"));
    m_keepGoing->setText(tr("Mark the word as incorrect and &keep going"));

    m_errorActionSettings->addButton(m_redo, ErrorAction::Redo);
    m_errorActionSettings->addButton(m_keepGoing, ErrorAction::KeepGoing);
    m_errorActionSettings
        ->button(settings.value("MemorizeEdit/errorAction", ErrorAction::Redo).toInt())
        ->setChecked(true);

#ifndef Q_OS_WASM // skip the unecessary stuff
    m_saveVerse->setText(tr("Load &last verse"));
    m_randVerse->setText(tr("Load &random verse"));
    m_setVerse->setText(tr("Load a &set verse"));
    m_chooseSetVerse->setText(tr("&Choose verse..."));

    // only let the user choose the default reference if the user has selected to load it
    m_chooseSetVerse->setEnabled(m_setVerse->isChecked());

    m_verseLoadSettings->addButton(m_saveVerse, VerseLoadOption::Saved);
    m_verseLoadSettings->addButton(m_randVerse, VerseLoadOption::Random);
    m_verseLoadSettings->addButton(m_setVerse, VerseLoadOption::Set);
    m_verseLoadSettings->button(settings.value("ChooseReferenceWidget/verseLoadOption", 1).toInt())
        ->setChecked(true);

    m_shouldSaveWindowSize->setText(tr("Save last set window size"));
    m_shouldSaveWindowSize->setChecked(settings.value("MainWindow/saveWinSize", true).toBool());
#endif // Q_OS_WASM

    m_reset->setText(tr("&Reset all settings..."));

    // Memorization tab
    auto errorActionGroupLayout = new QVBoxLayout;
    errorActionGroupLayout->addWidget(m_redo);
    errorActionGroupLayout->addWidget(m_keepGoing);
    errorActionGroupLayout->addStretch();

    auto errorActionGroup = new QGroupBox;
    errorActionGroup->setTitle(tr("Set action when the wrong key is typed"));
    errorActionGroup->setLayout(errorActionGroupLayout);

    auto memorizationLayout = new QVBoxLayout;
    memorizationLayout->addWidget(errorActionGroup);
    memorizationLayout->addStretch();

    auto memorization = new QWidget;
    memorization->setLayout(memorizationLayout);

#ifndef Q_OS_WASM // skip the unecessary stuff
    // Display tab
    auto displayLayout = new QVBoxLayout;
    displayLayout->addWidget(m_shouldSaveWindowSize);
    displayLayout->addStretch();
    auto display = new QWidget;
    display->setLayout(displayLayout);

    // Startup tab
    auto loadVerseGroupLayout = new QGridLayout;
    loadVerseGroupLayout->addWidget(m_saveVerse, 0, 0);
    loadVerseGroupLayout->addWidget(m_randVerse, 1, 0);
    loadVerseGroupLayout->addWidget(m_setVerse, 2, 0);
    loadVerseGroupLayout->addWidget(m_chooseSetVerse, 2, 1);

    auto loadVerseGroup = new QGroupBox;
    loadVerseGroup->setTitle(tr("Set what verse to load on startup"));
    loadVerseGroup->setLayout(loadVerseGroupLayout);

    auto startupLayout = new QVBoxLayout;
    startupLayout->addWidget(loadVerseGroup);
    startupLayout->addStretch();

    auto startup = new QWidget;
    startup->setLayout(startupLayout);
#endif // Q_OS_WASM

    // Other tab
    auto otherLayout = new QHBoxLayout;
    otherLayout->insertWidget(0, m_reset);
    otherLayout->insertStretch(1);
    auto other = new QWidget;
    other->setLayout(otherLayout);

    // connect the widgets
    connect(m_reset, &QPushButton::clicked, this, [this]() {
        if (QMessageBox::question(this, tr("Confirm reset"),
                                  tr("Are you sure you want to reset all settings? This will take "
                                     "effect immediately.")) == QMessageBox::StandardButton::Yes)
        {
            QSettings settings;
            settings.clear();
        }
    });

#ifndef Q_OS_WASM // skip the unecessary stuff
    connect(m_chooseSetVerse, &QPushButton::clicked, this, [this]() {
        QSettings settings;
        settings.beginGroup("ChooseReferenceWidget");
        SimpleRefChooser chooser{ this, settings.value("defaultBook", "").toString(),
                                  settings.value("defaultChapter", "").toString(),
                                  settings.value("defaultStartVerse", "").toString(),
                                  settings.value("defaultEndVerse", "").toString() };

        if (chooser.exec() == QDialog::DialogCode::Accepted)
        {
            settings.setValue("defaultBook", chooser.getBook());
            settings.setValue("defaultChapter", chooser.getChapter());
            settings.setValue("defaultStartVerse", chooser.getStartVerse());
            settings.setValue("defaultEndVerse", chooser.getEndVerse());
        }

        settings.endGroup();
    });
    connect(m_setVerse, &QRadioButton::toggled, m_chooseSetVerse, &QPushButton::setEnabled);
#endif // Q_OS_WASM

    auto buttonBox =
        new QDialogButtonBox{ QDialogButtonBox::StandardButton::Ok | QDialogButtonBox::Apply |
                              QDialogButtonBox::StandardButton::Cancel };
    connect(buttonBox->button(QDialogButtonBox::StandardButton::Ok), &QPushButton::clicked, this,
            &SettingsDialog::ok);
    connect(buttonBox->button(QDialogButtonBox::StandardButton::Apply), &QPushButton::clicked, this,
            &SettingsDialog::apply);
    connect(buttonBox->button(QDialogButtonBox::StandardButton::Cancel), &QPushButton::clicked,
            this, &SettingsDialog::reject);

    // add the widgets
    m_tabs->addTab(memorization, tr("&Memorization"));

#ifndef Q_OS_WASM // these tabs aren't needed with the stuff skipped in WASM
    m_tabs->addTab(display, tr("&Display"));
    m_tabs->addTab(startup, tr("&Startup"));
#endif // Q_OS_WASM

    m_tabs->addTab(other, tr("&Other settings"));

    auto layout = new QGridLayout;
    layout->addWidget(m_tabs, 0, 0);
    layout->addWidget(buttonBox, 1, 0);

    setLayout(layout);
}

void SettingsDialog::ok()
{
    apply();
    accept();
}

void SettingsDialog::apply()
{
    QSettings settings;

    settings.beginGroup("MemorizeEdit");
    settings.setValue("errorAction", m_errorActionSettings->checkedId());
    settings.endGroup(); // MemorizeEdit

#ifndef Q_OS_WASM // skip the unecessary stuff
    settings.beginGroup("ChooseReferenceWidget");

    settings.setValue("verseLoadOption", m_verseLoadSettings->checkedId());
    settings.endGroup(); // ChooseReferenceWidget

    settings.beginGroup("MainWindow");
    settings.setValue("saveWinSize", m_shouldSaveWindowSize->isChecked());
    settings.endGroup(); // MainWindow
#endif // Q_OS_WASM

    // TODO: disable the apply button when settings haven't been changed
    //	m_apply->setDisabled(true);
}
