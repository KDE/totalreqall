// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com
// SPDX-License-Identifier: BSD-2-Clause

#ifndef APPINFO_H
#define APPINFO_H

#include <QString>
#include <QVersionNumber>
#include <QApplication> // for tr()

namespace TotalReqall
{
    const QVersionNumber appVersion{ 0, 5, 0 };
	const QString appName{ QApplication::tr("TotalReqall Bible Memory") };
	const QString organizationName{ QApplication::tr("TotalReqall") };
	const QString organizationDomain{ "lorendb.github.io" };
}

#endif // APPINFO_H
