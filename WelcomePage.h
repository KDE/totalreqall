// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com
// SPDX-License-Identifier: BSD-2-Clause

#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QWidget>

class WelcomePage : public QWidget
{
    Q_OBJECT
public:
    explicit WelcomePage(QWidget *parent = nullptr);

signals:
    void bibleClicked();
    void customClicked();
};

#endif // WELCOMEPAGE_H
