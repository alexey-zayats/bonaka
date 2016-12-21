
#include "systemid_p.h"

namespace Utils {

SystemIdPrivate::SystemIdPrivate()
{
}

SystemId::SystemId(QObject *parent) :
    QObject(parent),
    d(new SystemIdPrivate())
{
}

const QString SystemId::getMachineName()
{
    return d->getMachineName();
}

unsigned short SystemId::getCpuHash()
{
    return d->getCpuHash();
}

unsigned short SystemId::getMacHash ()
{
    return d->getMacHash();
}

unsigned short SystemId::getVolumeHash ()
{
    return d->getMacHash();
}

}
