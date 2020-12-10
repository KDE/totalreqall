// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-License-Identifier: BSD-2-Clause

#ifndef APPINFO_H
#define APPINFO_H

#include <QString>
#include <QVersionNumber>

namespace TotalReqall
{
    const QVersionNumber appVersion{ ${CMAKE_PROJECT_VERSION_MAJOR},
	                                 ${CMAKE_PROJECT_VERSION_MINOR},
									 ${CMAKE_PROJECT_VERSION_PATCH} };
    const QString appName{ "TotalReqall" };
    const QString organizationName{ "KDE" };
    const QString organizationDomain{ "org.kde.totalreqall" };
} // namespace TotalReqall

#endif // APPINFO_H
