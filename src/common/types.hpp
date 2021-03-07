#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>

using guid_t = int;

using status_t = int;
#define IS_SUCCESS(x) for(;;){if (x >= 0) break; goto exit;}

using hostId_t = int;
using hostAddress_t = std::string;
using hostName_t = std::string;
using port_t = int;
using packageId_t  = int;
using dataVolume_t  = int; // in Bytes
using ttl_t = int;

enum class TransportProto : int
{
    TCP = 0,
    UDP,
    ICMP
};

enum class Priority : int
{
    NONE    = -1,

    LOW     = 1,
    MEDIUM  = 2,
    HIGH    = 3
};

#endif