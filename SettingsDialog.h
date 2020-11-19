#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QTabWidget>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>

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
	QCheckBox *m_swapRefChooserBtns;
#endif // Q_OS_WASM

	QPushButton *m_reset;

	QPushButton *m_ok;
	QPushButton *m_apply;
	QPushButton *m_cancel;
};

#endif // SETTINGSDIALOG_H
