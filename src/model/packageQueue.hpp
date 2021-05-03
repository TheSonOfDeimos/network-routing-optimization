#ifndef PACKAGE_QUEUE_HPP
#define PACKAGE_QUEUE_HPP

#include <list>
#include <mutex>

#include "types.hpp"
#include "package.hpp"

class Statistic;

enum class QueuePushRule : int
{
    FRONT = 0,
    BACK,
    PRIORITY,
    RANDOM
};

enum class QueuePopRule : int
{
    FRONT = 0,
    BACK,
    PRIORITY,
    RANDOM
};

enum class QueueDropRule : int
{
    LAST = 0, // Drop elements from the end to insert the new one
    FIRST,    // Drop elements from the head to insert  the new one
    NEW,      // Drop every new element
    RANDOM    // Drop elements randomly to insert  the new one
};

// Thread-safe package queue
class PackageQueue
{
public:
    PackageQueue(std::shared_ptr<Statistic> stat, hostAddress_t nodeAddr, dataVolume_t volume, QueuePushRule pushRule = QueuePushRule::BACK, QueuePopRule popRule = QueuePopRule::FRONT, QueueDropRule dropRule = QueueDropRule::LAST);

    status_t push(packagePtr_t package);
    packagePtr_t pop();

    double getPacketLoss();
    double getPing();

private:
    dataVolume_t volume();
    status_t insert(packagePtr_t package);
    status_t drop(dataVolume_t volumeToReclaim);

private:
    dataVolume_t m_totalVolume; // byte
    QueuePushRule m_pushRule;
    QueuePopRule m_popRule;
    QueueDropRule m_dropRule;
    std::list<packagePtr_t> m_queue;
    hostAddress_t m_nodeAddr;
    std::shared_ptr<Statistic> m_statistic;
    std::mutex m_mtx;

    std::vector<bool> m_drops; // vector of size m_measureFrame, true - packet droped, false - packet not droped
    std::vector<double> m_pings; // vector of size m_measureFrame with pings of every packet
    int m_measureFrame = 100;
};

#endif
