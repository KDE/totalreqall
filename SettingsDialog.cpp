#include "SettingsDialog.h"

#include <QMessageBox>
#include <QSettings>
#include <QGridLayout>
#include <QHBoxLayout>

SettingsDialog::SettingsDialog(QDialog *parent)
    : QDialog{ parent },
      m_tabs{ new QTabWidget },
      m_shouldSaveRef{ new QCheckBox },
      m_shouldSaveWindowSize{ new QCheckBox },
      m_swapRefChooserBtns{ new QCheckBox },
      m_reset{ new QPushButton },
      m_ok{ new QPushButton },
      m_apply{ new QPushButton },
      m_cancel{ new QPushButton }
{
	this->setWindowTitle(tr("Settings"));

	// get our settings object
	QSettings settings;

	// set up the widgets
	m_reset->setText(tr("Reset all settings..."));
	m_shouldSaveRef->setText(tr("Save last selected reference"));
	m_shouldSaveRef->setChecked(settings.value("ChooseReferenceWidget/saveLastRef", true).toBool());
	m_shouldSaveWindowSize->setText(tr("Save last set window size"));
	m_shouldSaveWindowSize->setChecked(settings.value("MainWindow/saveWinSize", true).toBool());
	m_swapRefChooserBtns->setText(tr("Swap Memorize and Display buttons"));
	m_swapRefChooserBtns->setChecked(settings.value("ChooseReferenceWidget/swapButtons", false).toBool());
	m_swapRefChooserBtns->setToolTip(tr("This will take effect after a restart."));
	m_ok->setText(tr("OK"));
	m_apply->setText(tr("Apply"));
	m_cancel->setText(tr("Cancel"));

	auto displayLayout = new QGridLayout;
	displayLayout->addWidget(m_swapRefChooserBtns, 0, 0);
	displayLayout->addWidget(m_shouldSaveWindowSize, 1, 0);
	auto display = new QWidget;
	display->setLayout(displayLayout);

	auto saveLayout = new QGridLayout;
	saveLayout->addWidget(m_shouldSaveRef, 0, 0);
	auto saveControls = new QWidget;
	saveControls->setLayout(saveLayout);

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
	connect(m_ok, &QPushButton::clicked, this, &SettingsDialog::ok);
	connect(m_apply, &QPushButton::clicked, this, &SettingsDialog::apply);
	connect(m_cancel, &QPushButton::clicked, this, &SettingsDialog::reject);

	// add the widgets
	m_tabs->addTab(display, tr("&Display"));
	m_tabs->addTab(saveControls, tr("&Saved data"));
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

	settings.beginGroup("ChooseReferenceWidget");
	settings.setValue("saveLastRef", m_shouldSaveRef->isChecked());

	// warn the user that a restart is needed
	if (settings.value("swapButtons", false).toBool() != m_swapRefChooserBtns->isChecked())
		QMessageBox::information(this, tr("Restart program"), tr("The buttons will be swapped after the program is restarted."));

	settings.setValue("swapButtons", m_swapRefChooserBtns->isChecked());
	settings.endGroup();

	settings.beginGroup("MainWindow");
	settings.setValue("saveWinSize", m_shouldSaveWindowSize->isChecked());
	settings.endGroup();

	// TODO: disable the apply button when settings haven't been changed
//	m_apply->setDisabled(true);
}
