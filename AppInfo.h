#ifndef APPINFO_H
#define APPINFO_H

#include <QString>
#include <QVersionNumber>
#include <QApplication> // for tr()

namespace TotalReqall
{
    const QVersionNumber appVersion{ 0, 4, 0 };
	const QString appName{ QApplication::tr("TotalReqall Bible Memory") };
	const QString organizationName{ QApplication::tr("TotalReqall") };
	const QString organizationDomain{ "lorendb.github.io" };
}

#endif // APPINFO_H
