// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com
// SPDX-License-Identifier: BSD-2-Clause

#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QCommandLinkButton>
#include <QWidget>

class WelcomePage : public QWidget
{
    Q_OBJECT
public:
    explicit WelcomePage(QWidget *parent = nullptr);

signals:
    void savedClicked();
    void bibleClicked();
    void customClicked();

public slots:
    void refresh();

private:
    // this is necessary because we sometimes need to change the disabled status of this button
    QCommandLinkButton *m_saved;
};

#endif // WELCOMEPAGE_H
