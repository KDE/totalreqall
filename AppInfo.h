#ifndef APPINFO_H
#define APPINFO_H

#include <QString>
#include <QApplication> // for tr()

namespace TotalReqall
{
    const QString version{ QApplication::tr("0.0.1") };
	const QString appName{ QApplication::tr("TotalReqall") };
}

#endif // APPINFO_H
