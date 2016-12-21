#ifndef SYSTEMID_H
#define SYSTEMID_H

#include <QObject>

#include "utils_global.h"

namespace Utils {

class SystemIdPrivate;

class UTILS_EXPORT SystemId : public QObject
{
    Q_OBJECT
public:
    explicit SystemId(QObject *parent = 0);

    const QString getMachineName();
    unsigned short getCpuHash();
    unsigned short getVolumeHash();
    unsigned short getMacHash();

private:
#ifdef Q_OSW_WIN
    unsigned short hashMacAddress( PIP_ADAPTER_INFO info )
#else
    unsigned short hashMacAddress( unsigned char* mac );
#endif
#ifndef Q_OS_MACX
    void getCpuid( unsigned int* p, unsigned int ax );
#endif
    SystemIdPrivate *d;
};

}

#endif // SYSTEMID_H
