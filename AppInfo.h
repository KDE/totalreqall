#ifndef APPINFO_H
#define APPINFO_H

#include <QString>
#include <QApplication> // for tr()

namespace TotalReqall
{
    const QString version{ QApplication::tr("0.2.0") };
	const QString appName{ QApplication::tr("TotalReqall Bible Memory") };
	const QString organizationName{ QApplication::tr("TotalReqall") };
	const QString organizationDomain{ "lorendb.github.io" };
}

#endif // APPINFO_H
