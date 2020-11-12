#ifndef MARKDOWNDIALOG_H
#define MARKDOWNDIALOG_H

#include <QDialog>

class MarkdownDialog : public QDialog
{
	Q_OBJECT
public:
	MarkdownDialog(QWidget *, QString, QString, bool = true);
};

#endif // MARKDOWNDIALOG_H
