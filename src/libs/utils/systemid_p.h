#ifndef SYSTEMID_P_H
#define SYSTEMID_P_H

#include "systemid.h"

namespace Utils {

class UTILS_EXPORT SystemIdPrivate
{
public:
    SystemIdPrivate();

    const QString getMachineName();
    unsigned short getCpuHash();
    unsigned short getVolumeHash();
    unsigned short getMacHash();

private:
#ifndef Q_OS_WIN
    unsigned short hashMacAddress( unsigned char* mac );
#endif
#ifndef Q_OS_MACX
    void getCpuid( unsigned int* p, unsigned int ax );
#endif
};

}

#endif // SYSTEMID_P_H
