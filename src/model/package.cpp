#include "package.hpp"

Package::Package(hostAddress_t src, hostAddress_t dst, dataVolume_t vol)
    : source(src),
    destination(dst),
    volume(vol)
{
    static packageId_t idCounter = 0;
    id = idCounter++;
}
