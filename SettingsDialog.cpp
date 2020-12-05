// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com
// SPDX-License-Identifier: BSD-2-Clause

#include "SettingsDialog.h"

#include "ChooseReferenceWidget.h"
#include "MemorizeWidget.h"
#include "SimpleRefChooser.h"
#include "UserSettings.h"
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
#ifndef Q_OS_WASM // skip the unecessary stuff
      m_verseLoadSettings{ new QButtonGroup },
      m_saveVerse{ new QRadioButton },
      m_randVerse{ new QRadioButton },
      m_setVerse{ new QRadioButton },
      m_chooseSetVerse{ new QPushButton },
      m_shouldSaveWindowSize{ new QCheckBox },
      m_bibleVersionLoad{ new QButtonGroup },
      m_loadLastBibleVersion{ new QRadioButton{ i18n("Load &last version") } },
      m_loadRandomBibleVersion{ new QRadioButton{ i18n("Load &random version") } },
      m_loadDefaultBibleVersion{ new QRadioButton{ i18n("Load &set version") } },
      m_defaultBibleVersion{ new QComboBox },
#endif // Q_OS_WASM
      m_tabs{ new QTabWidget },
      m_errorActionSettings{ new QButtonGroup },
      m_redo{ new QRadioButton },
      m_keepGoing{ new QRadioButton }
{
    this->setWindowTitle(i18n("Settings"));

    m_tabs->setUsesScrollButtons(false);

    // get our settings object
    QSettings qsettings;
    auto settings = UserSettings::global();

    // set up the widgets
    m_redo->setText(i18n("&Retype the word"));
    m_keepGoing->setText(i18n("Mark the word as incorrect and &keep going"));

    m_errorActionSettings->addButton(m_redo, ErrorAction::Redo);
    m_errorActionSettings->addButton(m_keepGoing, ErrorAction::KeepGoing);
    m_errorActionSettings
        ->button(qsettings.value("MemorizeEdit/errorAction", ErrorAction::Redo).toInt())
        ->setChecked(true);

#ifndef Q_OS_WASM // skip the unecessary stuff
    m_saveVerse->setText(i18n("Load &last verse"));
    m_randVerse->setText(i18n("Load &random verse"));
    m_setVerse->setText(i18n("Load a &set verse"));
    m_chooseSetVerse->setText(i18n("&Choose verse..."));

    m_verseLoadSettings->addButton(m_saveVerse, static_cast<int>(VerseLoadOption::Last));
    m_verseLoadSettings->addButton(m_randVerse, static_cast<int>(VerseLoadOption::Random));
    m_verseLoadSettings->addButton(m_setVerse, static_cast<int>(VerseLoadOption::Set));
    m_verseLoadSettings->button(static_cast<int>(settings->verseLoadOption()))->setChecked(true);

    // only let the user choose the default reference if the user has selected to load it
    m_chooseSetVerse->setEnabled(m_setVerse->isChecked());

    m_shouldSaveWindowSize->setText(i18n("Save last set window size"));
    m_shouldSaveWindowSize->setChecked(settings->saveWinSize());

    m_bibleVersionLoad->addButton(m_loadLastBibleVersion,
                                  static_cast<int>(BibleVersionLoadOption::Last));
    m_bibleVersionLoad->addButton(m_loadRandomBibleVersion,
                                  static_cast<int>(BibleVersionLoadOption::Random));
    m_bibleVersionLoad->addButton(m_loadDefaultBibleVersion,
                                  static_cast<int>(BibleVersionLoadOption::Set));
    m_bibleVersionLoad->button(static_cast<int>(settings->bibleVersionLoadOption()))
        ->setChecked(true);

    m_defaultBibleVersion->setEnabled(m_loadDefaultBibleVersion->isChecked());

    sword::SWMgr mgr;
    for (auto item : mgr.getModules())
    {
        auto type = item.second->getType();
        if (strcmp(type, sword::SWMgr::MODTYPE_BIBLES) == 0)
            m_defaultBibleVersion->addItem(item.first.c_str());
    }
    m_defaultBibleVersion->setCurrentText(settings->defaultBibleVersion());
#endif // Q_OS_WASM

    // Memorization tab
    auto errorActionGroupLayout = new QVBoxLayout;
    errorActionGroupLayout->addWidget(m_redo);
    errorActionGroupLayout->addWidget(m_keepGoing);
    errorActionGroupLayout->addStretch();

    auto errorActionGroup = new QGroupBox;
    errorActionGroup->setTitle(i18n("Set action when the wrong key is typed"));
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
    loadVerseGroup->setTitle(i18n("Set what verse to load on startup"));
    loadVerseGroup->setLayout(loadVerseGroupLayout);

    auto loadBibleVersionLayout = new QGridLayout;
    loadBibleVersionLayout->addWidget(m_loadLastBibleVersion, 0, 0);
    loadBibleVersionLayout->addWidget(m_loadRandomBibleVersion, 1, 0);
    loadBibleVersionLayout->addWidget(m_loadDefaultBibleVersion, 2, 0);
    loadBibleVersionLayout->addWidget(m_defaultBibleVersion, 2, 1);

    auto loadBibleVersion = new QGroupBox{ i18n("Set what Bible version to load on startup") };
    loadBibleVersion->setLayout(loadBibleVersionLayout);

    auto startupLayout = new QVBoxLayout;
    startupLayout->addWidget(loadVerseGroup);
    startupLayout->addWidget(loadBibleVersion);
    startupLayout->addStretch();

    auto startup = new QWidget;
    startup->setLayout(startupLayout);
#endif // Q_OS_WASM

    // connect the widgets
#ifndef Q_OS_WASM // skip the unecessary stuff
    connect(m_chooseSetVerse, &QPushButton::clicked, this, [this]() {
        auto settings = UserSettings::global();
        SimpleRefChooser chooser{ this, settings->defaultBook(), settings->defaultChapter(),
                                  settings->defaultStartVerse(), settings->defaultEndVerse() };
        if (chooser.exec() == QDialog::DialogCode::Accepted)
        {
            settings->setDefaultBook(chooser.getBook());
            settings->setDefaultChapter(chooser.getChapter());
            settings->setDefaultStartVerse(chooser.getStartVerse());
            settings->setDefaultEndVerse(chooser.getEndVerse());
        }
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
    m_tabs->addTab(memorization, i18n("&Memorization"));

#ifndef Q_OS_WASM // these tabs aren't needed with the stuff skipped in WASM
    m_tabs->addTab(display, i18n("&Display"));
    m_tabs->addTab(startup, i18n("&Startup"));
#endif // Q_OS_WASM

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
    auto settings = UserSettings::global();

    settings->setMemorizeErrorAction(static_cast<ErrorAction>(m_errorActionSettings->checkedId()));

#ifndef Q_OS_WASM // skip the unecessary stuff
    settings->setVerseLoadOption(static_cast<VerseLoadOption>(m_verseLoadSettings->checkedId()));
    settings->setBibleVersionLoadOption(
        static_cast<BibleVersionLoadOption>(m_bibleVersionLoad->checkedId()));
    if (m_bibleVersionLoad->checkedId() == static_cast<int>(BibleVersionLoadOption::Set))
        settings->setDefaultBibleVersion(m_defaultBibleVersion->currentText());

    settings->setSaveWinSize(m_shouldSaveWindowSize->isChecked());
#endif // Q_OS_WASM

    // TODO: disable the apply button when settings haven't been changed
    //	m_apply->setDisabled(true);
}
