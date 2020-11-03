#include "SettingsDialog.h"

#include <QMessageBox>
#include <QSettings>

SettingsDialog::SettingsDialog(QDialog *parent)
    : QDialog{ parent },
      m_reset{ new QPushButton },
//      m_chooseRefWidgetCtrls{ new QGroupBox },
//      m_mainWindowCtrls{ new QGroupBox },
      m_layout{ new QGridLayout{ this } }
{
	m_reset->setText(tr("Reset all settings..."));
	connect(m_reset, &QPushButton::clicked, this, [this]() {
		if (QMessageBox::question(this, tr("Confirm reset"), tr("Are you sure you want to reset all settings?")) == QMessageBox::StandardButton::Yes)
		{
			QSettings settings;
			settings.clear();
		}
	});

	// add the widgets
//	m_layout->addWidget(m_chooseRefWidgetCtrls, 0, 0);
//	m_layout->addWidget(m_mainWindowCtrls, 0, 1);
	m_layout->addWidget(m_reset, 1, 0);
}
