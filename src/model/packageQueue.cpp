#include "packageQueue.hpp"


status_t PackageQueue::push(packagePtr_t package)
{
    m_queue.push_back(std::move(package));
    return 0;
}

packagePtr_t PackageQueue::pop()
{
    packagePtr_t pack = std::move(m_queue.front());
    m_queue.pop_back();
    return pack;
}
