#include "packageQueue.hpp"

#include  <numeric>

#include "statistic.hpp"

PackageQueue::PackageQueue(std::shared_ptr<Statistic> stat, hostAddress_t nodeAddr, dataVolume_t volume, QueuePushRule pushRule, QueuePopRule popRule, QueueDropRule dropRule)
    : m_totalVolume(volume * 1e6),
      m_pushRule(pushRule),
      m_popRule(popRule),
      m_dropRule(dropRule),
      m_nodeAddr(nodeAddr),
      m_statistic(stat)
{
}

status_t PackageQueue::push(packagePtr_t package)
{
    status_t status = ERROR_OK;
    std::lock_guard<std::mutex> lock(m_mtx);

    EXIT_IF(package == nullptr, ERROR_NO_EFFECT);
    EXIT_IF(m_totalVolume < package->volume || package->volume == 0, ERROR_NO_EFFECT);
    {
        dataVolume_t vol = volume();
        if ((vol + package->volume) > m_totalVolume)
        {
            RUN(drop((vol + package->volume) - m_totalVolume));
            EXIT_IF(status == ERROR_NO_EFFECT, ERROR_NO_EFFECT);
        }

        package->inQueue = Time::instance().get();
        RUN(insert(std::move(package)));
    }

exit:
    return status;
}

packagePtr_t PackageQueue::pop()
{
    packagePtr_t pack = nullptr;
    std::lock_guard<std::mutex> lock(m_mtx);
    if (m_queue.size() == 0)
    {
        return pack;
    }

    switch (m_popRule)
    {
    case QueuePopRule::FRONT:
    {
        pack = std::move(m_queue.front());
        m_queue.pop_front();
        break;
    }
    case QueuePopRule::BACK:
    {
        pack = std::move(m_queue.back());
        m_queue.pop_back();
        break;
    }
    case QueuePopRule::RANDOM:
    {
        NOT_IMPLEMENTED_YET;
        break;
    }
    case QueuePopRule::PRIORITY:
    {
        NOT_IMPLEMENTED_YET;
        break;
    }
    }

    pack->outQueue = Time::instance().get();
    m_pings.push_back((double)(pack->outQueue - pack->inQueue) / g_oneMillisecond);
    if (m_pings.size() > (std::size_t)m_measureFrame) m_pings.erase(m_pings.begin());
    m_drops.push_back(false);
    if (m_drops.size() > (std::size_t)m_measureFrame) m_drops.erase(m_drops.begin());

    return pack;
}


double PackageQueue::getPacketLoss()
{
    if (m_drops.size() == 0) return 0;
    double drop = 0;
    for (std::size_t i = 0; i < m_drops.size(); i++)
    {
        if (m_drops.at(i) == true)
        {
            drop++;
        }
    }
    drop = drop / m_drops.size() * 100;
    return drop;
}

double PackageQueue::getPing()
{
    if (m_pings.size() == 0) return 0;
    double ping = std::accumulate(m_pings.begin(), m_pings.end(), 0.0, std::plus<double>());
    ping /= m_pings.size();
    return ping;
}

dataVolume_t PackageQueue::volume()
{
    dataVolume_t volume = 0;
    for (auto &pack : m_queue)
    {
        volume += pack->volume;
    }

    return volume;
}

status_t PackageQueue::insert(packagePtr_t package)
{
    status_t status = ERROR_OK;

    EXIT_IF(m_totalVolume < package->volume || package->volume == 0, ERROR_NO_EFFECT);
    EXIT_IF((volume() + package->volume) > m_totalVolume, ERROR_LOGIC);

    switch (m_pushRule)
    {
    case QueuePushRule::FRONT:
    {
        m_queue.push_front(std::move(package));
        break;
    }
    case QueuePushRule::BACK:
    {
        m_queue.push_back(std::move(package));
        break;
    }
    case QueuePushRule::PRIORITY:
    {
        NOT_IMPLEMENTED_YET;
        break;
    }
    case QueuePushRule::RANDOM:
    {
        NOT_IMPLEMENTED_YET;
        break;
    }
    }

exit:
    return status;
}

status_t PackageQueue::drop(dataVolume_t volumeToReclaim)
{
    status_t status = ERROR_OK;

    dataVolume_t reclaimedVol = 0;
    switch (m_dropRule)
    {
    case QueueDropRule::LAST:
    {
        m_queue.erase(m_queue.begin(), (std::remove_if(m_queue.rbegin(), m_queue.rend(), [&reclaimedVol, volumeToReclaim, this](auto &ex) {
                                            if (reclaimedVol <= volumeToReclaim)
                                            {
                                                reclaimedVol += ex->volume;
                                                ex->outSystem = Time::instance().get();
                                                ex->outQueue = Time::instance().get();
                                                ex->inProcess = Time::instance().get();
                                                ex->outProcess = Time::instance().get();
                                                ex->isReached = false;
                                                ex->lastNode = m_nodeAddr;
                                                m_statistic->report(std::move(ex));
                                                m_drops.push_back(true);
                                                if (m_drops.size() > (std::size_t)m_measureFrame) m_drops.erase(m_drops.begin());
                                                return true;
                                            }
                                            return false;
                                        }).base()));
        break;
    }
    case QueueDropRule::FIRST:
    {
        NOT_IMPLEMENTED_YET;
        break;
    }
    case QueueDropRule::NEW:
    {
        EXIT(ERROR_NO_EFFECT);
        break;
    }
    case QueueDropRule::RANDOM:
    {
        NOT_IMPLEMENTED_YET;
        break;
    }
    }

    EXIT_IF(reclaimedVol < volumeToReclaim, ERROR_LOGIC);

exit:
    return status;
}
