#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/resource.h>
#include <sys/utsname.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <assert.h>
#include <ifaddrs.h>
#include <net/if.h>

#include <mach-o/arch.h>

#include "systemid_p.h"

namespace Utils {

const QString SystemIdPrivate::getMachineName()
{
    static struct utsname u;

    if ( uname( &u ) < 0 )
    {
        assert(0);
        return QLatin1String("unknown");
    }

    return QLatin1String(u.nodename);
}

unsigned short SystemIdPrivate::getCpuHash()
{
    const NXArchInfo* info = NXGetLocalArchInfo();
    unsigned short val = 0;
    val += (unsigned short)info->cputype;
    val += (unsigned short)info->cpusubtype;
    return val;
}

unsigned short SystemIdPrivate::hashMacAddress( unsigned char* mac )
{
   unsigned short hash = 0;

   for ( unsigned int i = 0; i < 6; i++ )
   {
      hash += ( mac[i] << (( i & 1 ) * 8 ));
   }
   return hash;
}

unsigned short SystemIdPrivate::getMacHash()
{
    unsigned short mac = 0;

    struct ifaddrs* ifaphead;
    if ( getifaddrs( &ifaphead ) != 0 )
        assert(0);

    // iterate over the net interfaces
    struct ifaddrs* ifap;
    for ( ifap = ifaphead; ifap; ifap = ifap->ifa_next ) {
        struct sockaddr* sdl = (struct sockaddr*)ifap->ifa_addr;
        if ( sdl && ( sdl->sa_family == AF_LINK ) ) {
            mac = hashMacAddress( (unsigned char*)(sdl->sa_data)); //sdl->sdl_data) + sdl->sdl_nlen) );
            break;
        }
    }

    freeifaddrs( ifaphead );

   return mac;
}

unsigned short SystemIdPrivate::getVolumeHash()
{
    // we don't have a 'volume serial number' like on windows. Lets hash the system name instead.
    unsigned char* sysname = (unsigned char*)getMachineName().toLocal8Bit().data();
    unsigned short hash = 0;

    for ( unsigned int i = 0; sysname[i]; i++ )
        hash += ( sysname[i] << (( i & 1 ) * 8 ));

    return hash;
}

}
