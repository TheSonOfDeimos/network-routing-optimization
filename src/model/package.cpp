#include "package.hpp"

Package::Package(hostAddress_t src, hostAddress_t dst, dataVolume_t vol, TransportProto proto, Priority prior, ttl_t ttl)
    : source(src),
    destination(dst),
    volume(vol),
    protocol(proto),
    priority(prior),
    ttl(ttl)
{
    static packageId_t idCounter = 0;
    id = idCounter++;
}
