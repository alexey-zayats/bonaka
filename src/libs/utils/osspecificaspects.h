#ifndef OSSPECIFICASPECTS_H
#define OSSPECIFICASPECTS_H

#include "utils_global.h"

#include <QString>

#define WIN_EXE_SUFFIX ".exe"

namespace Utils {

// Add more as needed.
enum OsType { OsTypeWindows, OsTypeLinux, OsTypeMac, OsTypeOtherUnix, OsTypeOther };

class UTILS_EXPORT OsSpecificAspects
{
public:
    OsSpecificAspects(OsType osType) : m_osType(osType) { }

    QString withExecutableSuffix(const QString &executable) const {
        QString finalName = executable;
        if (m_osType == OsTypeWindows)
            finalName += QLatin1String(WIN_EXE_SUFFIX);
        return finalName;
    }

    Qt::CaseSensitivity fileNameCaseSensitivity() const {
        return m_osType == OsTypeWindows || m_osType == OsTypeMac ? Qt::CaseInsensitive : Qt::CaseSensitive;
    }

    QChar pathListSeparator() const {
        return QLatin1Char(m_osType == OsTypeWindows ? ';' : ':');
    }

    Qt::KeyboardModifier controlModifier() const {
        return m_osType == OsTypeMac ? Qt::MetaModifier : Qt::ControlModifier;
    }

private:
    const OsType m_osType;
};

} // namespace Utils

#endif // OSSPECIFICASPECTS_H
