#ifndef PACKAGE_HPP
#define PACKAGE_HPP

#include "types.hpp"
#include "time.hpp"
#include "algorithmBase.hpp"

struct Package
{
    Package(hostAddress_t src, hostAddress_t dst, dataVolume_t vol);

    hostAddress_t source;
    hostAddress_t destination;
    dataVolume_t volume; // In bytes
    TransportProto protocol = TransportProto::UDP;
    Priority priority = Priority::NONE;
    ttl_t ttl = INT32_MAX;
    packageId_t id;

    std::vector<hostAddress_t> path = {};

    // Status
    modelTime_t inQueue = 0;
    modelTime_t outQueue = 0;
    modelTime_t inProcess = 0;
    modelTime_t outProcess = 0;

    modelTime_t inSystem = 0;
    modelTime_t outSystem = 0;

    bool isReached = false;
    hostAddress_t lastNode = -1;
};

using packagePtr_t = std::unique_ptr<Package>;

#endif
