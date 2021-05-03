#include "statistic.hpp"

status_t Statistic::report(const NodeCharacteristics &params)
{
    std::lock_guard<std::mutex> lock(m_mtx);

    if (m_nodeReports.find(params.addr) == m_nodeReports.end())
    {
        m_nodeReports[params.addr] = {};
    }

    m_nodeReports.at(params.addr).push_back({Time::instance().get(),
                                             {params.addr,
                                             params.ping,
                                             params.packetLoss,
                                             params.speed,
                                             params.roles}});

    onNodeReport(m_nodeReports.at(params.addr).back());
    return ERROR_OK;;
}

status_t Statistic::report(packagePtr_t package)
{
    std::lock_guard<std::mutex> lock(m_mtx);

    m_packagesBin.push_back(std::move(package));
    return ERROR_OK;;
}



status_t Statistic::subscribe(subId id, subscriptionCallback call, Filter filter)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    m_subscribers[id] = {call, filter};

    return ERROR_OK;
}

status_t Statistic::unsubscribe(subId id)
{
    m_subscribers.erase(id);
    return ERROR_OK;
}

status_t Statistic::onNodeReport(const record_t &rec)
{
    status_t status = ERROR_OK;

    record_t localRec = rec;
    for (auto& sub : m_subscribers)
    {
        if (!sub.second.second.roles.empty())
            if (sub.second.second.roles != localRec.second.roles) continue;

        if (sub.second.second.addr != -1 && sub.second.second.addr != localRec.second.addr) continue;

        std::vector<hostAddress_t> hosts = requiredHosts(sub.second.second);
        if(hosts.size() == 0) continue;

        if (sub.second.second.addr == -1)
        {
            if (!isValidForTime(localRec.first)) continue;
            localRec = getAverage(rec.first, hosts);
        }

        if (localRec.second.ping <= -1) continue;

        double metricVal = -1;
        switch (sub.second.second.metric)
        {
        case MetricType::SPEED :
        {
            metricVal = localRec.second.speed;
            break;
        }
        case MetricType::PACKET_LOSS :
        {
            metricVal = localRec.second.packetLoss;
            break;
        }
        case MetricType::PING :
        {
            metricVal = localRec.second.ping;
            break;
        }
        }

        sub.second.first(localRec.first, metricVal);
    }

exit:
    return status;
}

record_t Statistic::getAverage(modelTime_t time, const std::vector<hostAddress_t>& hosts)
{
    int index = -1;

    // Find requird index
    for (std::size_t i = m_nodeReports.at(hosts.front()).size() - 1; i >= 0; i--)
    {
        if (m_nodeReports.at(hosts.front()).at(i).first == time)
        {
            index = i;
            break;
        }
    }

    if (index == -1) return {time, {-1, -1, -1, -1, {}}};

    Record avgRecord;
    avgRecord.addr = -1;
    avgRecord.speed = 0;
    avgRecord.packetLoss = 0;
    avgRecord.ping = 0;
    avgRecord.roles = m_nodeReports.at(hosts.front()).at(index).second.roles;

    for (auto addr : hosts)
    {
        auto list = m_nodeReports.find(addr);
        if (list == m_nodeReports.end()) return {time, {-1, -1, -1, -1, {}}};

        if (list->second.at(index).first != time) return {time, {-1, -1, -1, -1, {}}};

        avgRecord.speed += list->second.at(index).second.speed;
        avgRecord.packetLoss += list->second.at(index).second.packetLoss;
        avgRecord.ping += list->second.at(index).second.ping;
    }

    avgRecord.speed /= hosts.size();
    avgRecord.packetLoss /= hosts.size();
    avgRecord.ping /= hosts.size();

    return {time, avgRecord};
}


bool Statistic::isValidForTime(modelTime_t time)
{
    if (m_nodeReports.size() == 0) return false;

    int size = m_nodeReports.begin()->second.size();
    for (auto& list : m_nodeReports)
    {
        if (list.second.size() == 0) return false;
        if (list.second.size() != static_cast<std::size_t>(size) || list.second.size() < 2 || list.second.back().first < time)
        {
            return false;
        }
    }

    return true;
}

std::vector<hostAddress_t> Statistic::requiredHosts(const Filter &filter)
{
    std::vector<hostAddress_t> hosts;

    if (filter.addr != -1)
    {
        auto find = m_nodeReports.find(filter.addr);
        if (find != m_nodeReports.end() && m_nodeReports.at(filter.addr).front().second.roles == filter.roles)
        {
            hosts.push_back(filter.addr);
        }
    }
    else
    {
        for (auto& list : m_nodeReports)
        {
            if (filter.roles.empty())
            {
                hosts.push_back(list.first);
                continue;
            }
            if (list.second.front().second.roles == filter.roles)
            {
                hosts.push_back(list.first);
                continue;
            }

        }
    }

    return hosts;

}

bool Statistic::isReportSameSize()
{
    if (m_nodeReports.size() == 0) return false;

    int size = m_nodeReports.begin()->second.size();
    for (auto& list : m_nodeReports)
    {
        if (list.second.size() == 0) return false;
        if (list.second.size() != static_cast<std::size_t>(size) || list.second.size() < 2)
        {
            return false;
        }
    }

    return true;
}
