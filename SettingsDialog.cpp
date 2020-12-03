// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com
// SPDX-License-Identifier: BSD-2-Clause

#include "SettingsDialog.h"

#include "ChooseReferenceWidget.h"
#include "MemorizeWidget.h"
#include "SimpleRefChooser.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSettings>
#include <QVBoxLayout>
#include <swmgr.h>
#include <swmodule.h>

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
      m_bibleVersionLoad{ new QButtonGroup },
      m_loadLastBibleVersion{ new QRadioButton{ ki18n("Load &last version").toString() } },
      m_loadDefaultBibleVersion{ new QRadioButton{ ki18n("Load &set version").toString() } },
      m_defaultBibleVersion{ new QComboBox },
#endif // Q_OS_WASM
      m_reset{ new QPushButton }
{
    this->setWindowTitle(ki18n("Settings").toString());

    m_tabs->setUsesScrollButtons(false);

    // get our settings object
    QSettings settings;

    // set up the widgets
    m_redo->setText(ki18n("&Retype the word").toString());
    m_keepGoing->setText(ki18n("Mark the word as incorrect and &keep going").toString());

    m_errorActionSettings->addButton(m_redo, ErrorAction::Redo);
    m_errorActionSettings->addButton(m_keepGoing, ErrorAction::KeepGoing);
    m_errorActionSettings
        ->button(settings.value("MemorizeEdit/errorAction", ErrorAction::Redo).toInt())
        ->setChecked(true);

#ifndef Q_OS_WASM // skip the unecessary stuff
    m_saveVerse->setText(ki18n("Load &last verse").toString());
    m_randVerse->setText(ki18n("Load &random verse").toString());
    m_setVerse->setText(ki18n("Load a &set verse").toString());
    m_chooseSetVerse->setText(ki18n("&Choose verse...").toString());

    // only let the user choose the default reference if the user has selected to load it
    m_chooseSetVerse->setEnabled(m_setVerse->isChecked());

    m_verseLoadSettings->addButton(m_saveVerse, static_cast<int>(VerseLoadOption::Last));
    m_verseLoadSettings->addButton(m_randVerse, static_cast<int>(VerseLoadOption::Random));
    m_verseLoadSettings->addButton(m_setVerse, static_cast<int>(VerseLoadOption::Set));
    m_verseLoadSettings
        ->button(settings
                     .value("ChooseReferenceWidget/verseLoadOption",
                            static_cast<int>(VerseLoadOption::Last))
                     .toInt())
        ->setChecked(true);

    m_shouldSaveWindowSize->setText(ki18n("Save last set window size").toString());
    m_shouldSaveWindowSize->setChecked(settings.value("MainWindow/saveWinSize", true).toBool());

    m_bibleVersionLoad->addButton(m_loadLastBibleVersion,
                                  static_cast<int>(BibleVersionLoadOption::Last));
    m_bibleVersionLoad->addButton(m_loadDefaultBibleVersion,
                                  static_cast<int>(BibleVersionLoadOption::Set));
    m_bibleVersionLoad
        ->button(settings
                     .value("ChooseReferenceWidget/bibleVersionLoadOption",
                            static_cast<int>(BibleVersionLoadOption::Last))
                     .toInt())
        ->setChecked(true);

    m_defaultBibleVersion->setEnabled(m_loadDefaultBibleVersion->isChecked());

    sword::SWMgr mgr;
    for (auto item : mgr.getModules())
    {
        auto mod = item.second;

        auto type = mod->getType();
        if (strcmp(type, sword::SWMgr::MODTYPE_BIBLES) == 0)
            m_defaultBibleVersion->addItem(item.first.c_str());
    }
    m_defaultBibleVersion->setCurrentText(settings
                                              .value("ChooseReferenceWidget/defaultBibleVersion",
                                                     m_defaultBibleVersion->currentText())
                                              .toString());
#endif // Q_OS_WASM

    m_reset->setText(ki18n("&Reset all settings...").toString());

    // Memorization tab
    auto errorActionGroupLayout = new QVBoxLayout;
    errorActionGroupLayout->addWidget(m_redo);
    errorActionGroupLayout->addWidget(m_keepGoing);
    errorActionGroupLayout->addStretch();

    auto errorActionGroup = new QGroupBox;
    errorActionGroup->setTitle(ki18n("Set action when the wrong key is typed").toString());
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
    loadVerseGroup->setTitle(ki18n("Set what verse to load on startup").toString());
    loadVerseGroup->setLayout(loadVerseGroupLayout);

    auto loadBibleVersionLayout = new QGridLayout;
    loadBibleVersionLayout->addWidget(m_loadLastBibleVersion, 0, 0);
    loadBibleVersionLayout->addWidget(m_loadDefaultBibleVersion, 1, 0);
    loadBibleVersionLayout->addWidget(m_defaultBibleVersion, 1, 1);

    auto loadBibleVersion =
        new QGroupBox{ ki18n("Set what Bible version to load on startup").toString() };
    loadBibleVersion->setLayout(loadBibleVersionLayout);

    auto startupLayout = new QVBoxLayout;
    startupLayout->addWidget(loadVerseGroup);
    startupLayout->addWidget(loadBibleVersion);
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
        if (QMessageBox::question(this, ki18n("Confirm reset").toString(),
                                  ki18n("Are you sure you want to reset all settings? This will "
                                        "take "
                                        "effect immediately.")
                                      .toString()) == QMessageBox::StandardButton::Yes)
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
    connect(m_loadDefaultBibleVersion, &QRadioButton::toggled, m_defaultBibleVersion,
            &QComboBox::setEnabled);
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
    m_tabs->addTab(memorization, ki18n("&Memorization").toString());

#ifndef Q_OS_WASM // these tabs aren't needed with the stuff skipped in WASM
    m_tabs->addTab(display, ki18n("&Display").toString());
    m_tabs->addTab(startup, ki18n("&Startup").toString());
#endif // Q_OS_WASM

    m_tabs->addTab(other, ki18n("&Other settings").toString());

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
    settings.setValue("bibleVersionLoadOption", m_bibleVersionLoad->checkedId());
    if (m_bibleVersionLoad->checkedId() == static_cast<int>(BibleVersionLoadOption::Set))
        settings.setValue("defaultBibleVersion", m_defaultBibleVersion->currentText());

    settings.endGroup(); // ChooseReferenceWidget

    settings.beginGroup("MainWindow");
    settings.setValue("saveWinSize", m_shouldSaveWindowSize->isChecked());
    settings.endGroup(); // MainWindow
#endif // Q_OS_WASM

    // TODO: disable the apply button when settings haven't been changed
    //	m_apply->setDisabled(true);
}
