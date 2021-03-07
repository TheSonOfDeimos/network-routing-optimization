#ifndef PACKAGE_QUEUE_HPP
#define PACKAGE_QUEUE_HPP

#include <list>

#include "types.hpp"
#include "package.hpp"

class PackageQueue
{
public:
    status_t push(packagePtr_t package);
    packagePtr_t pop();

private:
    std::list<packagePtr_t> m_queue;

};

#endif
