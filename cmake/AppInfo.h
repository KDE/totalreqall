// SPDX-FileCopyrightText: 2020 Loren Burkholder <computersemiexpert@outlook.com>
// SPDX-License-Identifier: BSD-2-Clause

#ifndef APPINFO_H
#define APPINFO_H

#include <QString>
#include <QVersionNumber>

namespace TotalReqall
{
    const QVersionNumber appVersion{ ${CPACK_PACKAGE_VERSION_MAJOR}, ${CPACK_PACKAGE_VERSION_MINOR}, ${CPACK_PACKAGE_VERSION_PATCH} };
	const QString appName{ "TotalReqall" };
	const QString organizationName{ "KDE" };
	const QString organizationDomain{ "org.kde.totalreqall" };
} // namespace TotalReqall

#endif // APPINFO_H
