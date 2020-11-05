#include "SettingsDialog.h"

#include <QMessageBox>
#include <QSettings>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

#include "ChooseReferenceWidget.h"
#include "SimpleRefChooser.h"
#include "MemorizeEdit.h"

SettingsDialog::SettingsDialog(QDialog *parent)
    : QDialog{ parent },
      m_tabs{ new QTabWidget },
      m_errorActionSettings{ new QButtonGroup },
      m_redo{ new QRadioButton },
      m_keepGoing{ new QRadioButton },
      m_verseLoadSettings{ new QButtonGroup },
      m_saveVerse{ new QRadioButton },
      m_randVerse{ new QRadioButton },
      m_setVerse{ new QRadioButton },
      m_chooseSetVerse{ new QPushButton },
      m_shouldSaveWindowSize{ new QCheckBox },
      m_swapRefChooserBtns{ new QCheckBox },
      m_reset{ new QPushButton },
      m_ok{ new QPushButton },
      m_apply{ new QPushButton },
      m_cancel{ new QPushButton }
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
	m_errorActionSettings->button(settings.value("MemorizeEdit/errorAction", ErrorAction::Redo).toInt())->setChecked(true);

	m_saveVerse->setText(tr("Load &last verse"));
	m_randVerse->setText(tr("Load &random verse"));
	m_setVerse->setText(tr("Load a &set verse"));
	m_chooseSetVerse->setText(tr("&Choose verse..."));

	// only let the user choose the default reference if the user has selected to load it
	m_chooseSetVerse->setEnabled(m_setVerse->isChecked());

	m_verseLoadSettings->addButton(m_saveVerse, VerseLoadOption::Saved);
	m_verseLoadSettings->addButton(m_randVerse, VerseLoadOption::Random);
	m_verseLoadSettings->addButton(m_setVerse, VerseLoadOption::Set);
	m_verseLoadSettings->button(settings.value("ChooseReferenceWidget/verseLoadOption", 1).toInt())->setChecked(true);

	m_shouldSaveWindowSize->setText(tr("Save last set window size"));
	m_shouldSaveWindowSize->setChecked(settings.value("MainWindow/saveWinSize", true).toBool());

	m_swapRefChooserBtns->setText(tr("Swap Memorize and Display buttons"));
	m_swapRefChooserBtns->setChecked(settings.value("ChooseReferenceWidget/swapButtons", false).toBool());
	m_swapRefChooserBtns->setToolTip(tr("This will take effect after a restart."));

	m_reset->setText(tr("&Reset all settings..."));

	m_ok->setText(tr("OK"));
	m_apply->setText(tr("Apply"));
	m_cancel->setText(tr("Cancel"));

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

	// Display tab
	auto displayLayout = new QVBoxLayout;
	displayLayout->addWidget(m_swapRefChooserBtns);
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

	// Other tab
	auto otherLayout = new QHBoxLayout;
	otherLayout->insertWidget(0, m_reset);
	otherLayout->insertStretch(1);
	auto other = new QWidget;
	other->setLayout(otherLayout);

	auto okCancelLayout = new QHBoxLayout;
	okCancelLayout->insertStretch(0);
	okCancelLayout->insertWidget(1, m_ok);
	okCancelLayout->insertWidget(2, m_apply);
	okCancelLayout->insertWidget(3, m_cancel);

	// connect the widgets
	connect(m_reset, &QPushButton::clicked, this, [this]() {
		if (QMessageBox::question(this, tr("Confirm reset"), tr("Are you sure you want to reset all settings? This will take effect immediately."))
		        == QMessageBox::StandardButton::Yes)
		{
			QSettings settings;
			settings.clear();
		}
	});
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
	connect(m_ok, &QPushButton::clicked, this, &SettingsDialog::ok);
	connect(m_apply, &QPushButton::clicked, this, &SettingsDialog::apply);
	connect(m_cancel, &QPushButton::clicked, this, &SettingsDialog::reject);

	// add the widgets
	m_tabs->addTab(memorization, tr("&Memorization"));
	m_tabs->addTab(display, tr("&Display"));
	m_tabs->addTab(startup, tr("&Startup"));
	m_tabs->addTab(other, tr("&Other settings"));

	auto layout = new QGridLayout;
	layout->addWidget(m_tabs, 0, 0);
	layout->addLayout(okCancelLayout, 1, 0);

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

	settings.beginGroup("ChooseReferenceWidget");

	// warn the user that a restart is needed
	if (settings.value("swapButtons", false).toBool() != m_swapRefChooserBtns->isChecked())
		QMessageBox::information(this, tr("Restart program"), tr("The buttons will be swapped after the program is restarted."));

	settings.setValue("swapButtons", m_swapRefChooserBtns->isChecked());
	settings.setValue("verseLoadOption", m_verseLoadSettings->checkedId());
	settings.endGroup(); // ChooseReferenceWidget

	settings.beginGroup("MainWindow");
	settings.setValue("saveWinSize", m_shouldSaveWindowSize->isChecked());
	settings.endGroup(); // MainWindow

	// TODO: disable the apply button when settings haven't been changed
//	m_apply->setDisabled(true);
}
