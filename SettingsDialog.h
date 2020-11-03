#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QGroupBox>

class SettingsDialog : public QDialog
{
	Q_OBJECT
public:
	explicit SettingsDialog(QDialog *parent = nullptr);

signals:

private:
	QPushButton *m_reset;
//	QGroupBox *m_chooseRefWidgetCtrls;
//	QGroupBox *m_mainWindowCtrls;
	QGridLayout *m_layout;

};

#endif // SETTINGSDIALOG_H
