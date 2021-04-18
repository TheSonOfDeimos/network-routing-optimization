#ifndef PACKAGE_HPP
#define PACKAGE_HPP

#include "types.hpp"

struct Package
{
    Package(hostAddress_t src, hostAddress_t dst, dataVolume_t vol, TransportProto proto = TransportProto::UDP, Priority prior = Priority::NONE, ttl_t ttl = INT32_MAX);

    hostAddress_t source;
    hostAddress_t destination;
    dataVolume_t volume; // In bytes
    TransportProto protocol;
    Priority priority;
    ttl_t ttl;
    packageId_t id;
};

using packagePtr_t = std::unique_ptr<Package>;

#endif