#ifndef PACKAGE_PROCESSOR_HPP
#define PACKAGE_PROCESSOR_HPP

#include "types.hpp"
#include "package.hpp"
#include "time.hpp"

class PackageProcessor
{
public:
    PackageProcessor(double bandwidth);

    // Prosess package, processing duration depens on package volume and bandwidth
    status_t push(packagePtr_t pack);

    // Check if package ready, processing time elapsed
    bool isReady();
    packagePtr_t pop();

private:
    const double m_bandwidth; // bit/picosec
    packagePtr_t m_currentPackage;
    modelTime_t m_processingTime;
    Timer m_timer;
};

#endif
