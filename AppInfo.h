// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-License-Identifier: BSD-2-Clause

#ifndef APPINFO_H
#define APPINFO_H

#include <QApplication> // for tr()
#include <QString>
#include <QVersionNumber>

namespace TotalReqall
{
    const QVersionNumber appVersion{ 0, 5, 0 };
    const QString appName{ QApplication::tr("TotalReqall") };
    const QString organizationName{ QApplication::tr("TotalReqall") };
    const QString organizationDomain{ "lorendb.github.io" };
} // namespace TotalReqall

#endif // APPINFO_H
