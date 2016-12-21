
#ifndef HOSTOSINFO_H
#define HOSTOSINFO_H

#include "utils_global.h"

#include "osspecificaspects.h"

#include <QString>

#ifdef Q_OS_WIN
#define QTC_HOST_EXE_SUFFIX QTC_WIN_EXE_SUFFIX
#else
#define QTC_HOST_EXE_SUFFIX ""
#endif // Q_OS_WIN

namespace Utils {

class UTILS_EXPORT HostOsInfo
{
public:
    static inline OsType hostOs();

    enum HostArchitecture { HostArchitectureX86, HostArchitectureAMD64, HostArchitectureItanium,
                            HostArchitectureArm, HostArchitectureUnknown };
    static HostArchitecture hostArchitecture();

    static bool isWindowsHost() { return hostOs() == OsTypeWindows; }
    static bool isLinuxHost() { return hostOs() == OsTypeLinux; }
    static bool isMacHost() { return hostOs() == OsTypeMac; }
    static inline bool isAnyUnixHost();

    static QString withExecutableSuffix(const QString &executable)
    {
        return hostOsAspects().withExecutableSuffix(executable);
    }

    static void setOverrideFileNameCaseSensitivity(Qt::CaseSensitivity sensitivity);
    static void unsetOverrideFileNameCaseSensitivity();

    static Qt::CaseSensitivity fileNameCaseSensitivity()
    {
        return m_useOverrideFileNameCaseSensitivity
                ? m_overrideFileNameCaseSensitivity
                : hostOsAspects().fileNameCaseSensitivity();
    }

    static QChar pathListSeparator()
    {
        return hostOsAspects().pathListSeparator();
    }

    static Qt::KeyboardModifier controlModifier()
    {
        return hostOsAspects().controlModifier();
    }

    static bool canCreateOpenGLContext(QString *errorMessage);

private:
    static OsSpecificAspects hostOsAspects() { return OsSpecificAspects(hostOs()); }

    static Qt::CaseSensitivity m_overrideFileNameCaseSensitivity;
    static bool m_useOverrideFileNameCaseSensitivity;
};


OsType HostOsInfo::hostOs()
{
#if defined(Q_OS_WIN)
    return OsTypeWindows;
#elif defined(Q_OS_LINUX)
    return OsTypeLinux;
#elif defined(Q_OS_MAC)
    return OsTypeMac;
#elif defined(Q_OS_UNIX)
    return OsTypeOtherUnix;
#else
    return OsTypeOther;
#endif
}

bool HostOsInfo::isAnyUnixHost()
{
#ifdef Q_OS_UNIX
    return true;
#else
    return false;
#endif
}

} // namespace Utils

#endif // HOSTOSINFO_H
