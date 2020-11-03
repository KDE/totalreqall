#include "SettingsDialog.h"

#include <QMessageBox>
#include <QSettings>
#include <QGridLayout>
#include <QHBoxLayout>

SettingsDialog::SettingsDialog(QDialog *parent)
    : QDialog{ parent },
      m_tabs{ new QTabWidget },
      m_saveControls{ new QWidget },
      m_shouldSaveRef{ new QCheckBox },
      m_shouldSaveWindowSize{ new QCheckBox },
      m_other{ new QWidget },
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
	m_ok->setText(tr("OK"));
	m_apply->setText(tr("Apply"));
	m_cancel->setText(tr("Cancel"));

	auto saveLayout = new QGridLayout;
	saveLayout->addWidget(m_shouldSaveRef, 0, 0);
	saveLayout->addWidget(m_shouldSaveWindowSize, 1, 0);
	m_saveControls->setLayout(saveLayout);
//	m_saveControls->setTitle(tr("Saved data"));

	auto otherLayout = new QGridLayout;
	otherLayout->addWidget(m_reset, 0, 0);
	m_other->setLayout(otherLayout);
//	m_other->setTitle(tr("Other settings"));

	auto okCancelLayout = new QHBoxLayout;
	okCancelLayout->insertStretch(0);
	okCancelLayout->insertWidget(1, m_ok);
	okCancelLayout->insertWidget(2, m_apply);
	okCancelLayout->insertWidget(3, m_cancel);

	// connect the widgets
	connect(m_reset, &QPushButton::clicked, this, [this]() {
		if (QMessageBox::question(this, tr("Confirm reset"), tr("Are you sure you want to reset all settings? This will take effect immediately.")) == QMessageBox::StandardButton::Yes)
		{
			QSettings settings;
			settings.clear();
		}
	});
	connect(m_ok, &QPushButton::clicked, this, &SettingsDialog::ok);
	connect(m_apply, &QPushButton::clicked, this, &SettingsDialog::apply);
	connect(m_cancel, &QPushButton::clicked, this, &SettingsDialog::reject);

	// add the widgets
	m_tabs->addTab(m_saveControls, tr("&Saved data"));
	m_tabs->addTab(m_other, tr("Other settings"));

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
	settings.setValue("ChooseReferenceWidget/saveLastRef", m_shouldSaveRef->isChecked());
	settings.setValue("MainWindow/saveWinSize", m_shouldSaveWindowSize->isChecked());

	// TODO: disable the apply button when settings haven't been changed
//	m_apply->setDisabled(true);
}
