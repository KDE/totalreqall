#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QTabWidget>
#include <QCheckBox>

class SettingsDialog : public QDialog
{
	Q_OBJECT
public:
	explicit SettingsDialog(QDialog *parent = nullptr);

signals:

private slots:
	void ok();
	void apply();

private:
	QTabWidget *m_tabs;

	QCheckBox *m_shouldSaveRef;	
	QCheckBox *m_shouldSaveWindowSize;
	QCheckBox *m_swapRefChooserBtns;
	// TODO: add option for default ref to load

	QPushButton *m_reset;

	QPushButton *m_ok;
	QPushButton *m_apply;
	QPushButton *m_cancel;
};

#endif // SETTINGSDIALOG_H
