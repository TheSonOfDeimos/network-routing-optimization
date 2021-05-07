#include "statistic.hpp"

#include <numeric>

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
    onPacketReport(*m_packagesBin.back().get());
    return ERROR_OK;;
}

status_t Statistic::onNodeReport(const record_t &rec)
{
    if (isReportSameSize())
    {
        auto avgProvider = countLastAverage({RoleType::PROVIDER});
        if (avgProvider.second.speed >= 0) m_avgProvider.push_back(avgProvider);

        auto avgProducer = countLastAverage({RoleType::PRODUCER});
        if (avgProducer.second.speed >= 0) m_avgProducer.push_back(avgProducer);

        auto avgConsumer = countLastAverage({RoleType::CONSUMER});
        if (avgConsumer.second.speed >= 0) m_avgConsumer.push_back(avgConsumer);

        auto avgProviderProducer = countLastAverage({RoleType::PROVIDER, RoleType::PRODUCER});
        if (avgProviderProducer.second.speed >= 0) m_avgProviderProducer.push_back(avgProviderProducer);

        auto avgProviderConsumer = countLastAverage({RoleType::PROVIDER, RoleType::CONSUMER});
        if (avgProviderConsumer.second.speed >= 0)
        {
            m_avgProviderConsumer.push_back(avgProviderConsumer);
        }

        auto avgProducerConsumer = countLastAverage({RoleType::PRODUCER, RoleType::CONSUMER});
        if (avgProducerConsumer.second.speed >= 0) m_avgProducerConsumer.push_back(avgProducerConsumer);

        auto avgProviderProducerConsumer = countLastAverage({RoleType::PROVIDER, RoleType::PRODUCER, RoleType::CONSUMER});
        if (avgProviderProducerConsumer.second.speed >= 0) m_avgProviderProducerConsumer.push_back(avgProviderProducerConsumer);

        auto avgAll = countLastAverage({});
        if (avgAll.second.speed >= 0) m_avgAll.push_back(avgAll);
    }

    return ERROR_OK;
}

status_t Statistic::onPacketReport(const Package &package)
{
    if (package.isReached == false) return ERROR_OK;

    m_measurePacketTimeInSystem.push_back(package.outSystem - package.inSystem);
    if (m_measurePacketTimeInSystem.size() > static_cast<std::size_t>(m_measureFrame)) m_measurePacketTimeInSystem.erase(m_measurePacketTimeInSystem.begin());

    modelTime_t avgTimeInSystem = std::accumulate(m_measurePacketTimeInSystem.begin(), m_measurePacketTimeInSystem.end(), (modelTime_t)0, std::plus<modelTime_t>());
    avgTimeInSystem /= m_measurePacketTimeInSystem.size();
    m_avgPacketTimeInSystem.emplace_back(package.outSystem, avgTimeInSystem);

    return ERROR_OK;
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

record_t Statistic::countLastAverage(const std::vector<RoleType>& roles)
{
    std::vector<hostAddress_t> hosts;
    std::size_t index = m_nodeReports.begin()->second.size() - 1;

    Record avgLastRec;
    avgLastRec.addr = -1;
    avgLastRec.speed = 0;
    avgLastRec.packetLoss = 0;
    avgLastRec.ping = 0;
    avgLastRec.roles = roles;

    for (auto& list : m_nodeReports)
    {
        if (list.second.at(index).second.roles == roles || roles.empty())
        {
            hosts.emplace_back(list.first);
            avgLastRec.speed += list.second.at(index).second.speed;
            avgLastRec.packetLoss += list.second.at(index).second.packetLoss;
            avgLastRec.ping += list.second.at(index).second.ping;
        }
    }

    if (hosts.empty()) return {0, {-1, -1.0, -1.0, -1.0, {}}};

    avgLastRec.speed /= hosts.size();
    avgLastRec.packetLoss /= hosts.size();
    avgLastRec.ping /= hosts.size();

    return {m_nodeReports.at(hosts.front()).at(index).first, avgLastRec};
}

std::pair<std::vector<double>, std::vector<double>> Statistic::get(const Filter &filter)
{
    std::lock_guard<std::mutex> lock(m_mtx);

    recordsList_t* list = nullptr;
    std::vector<double> time = {}; // ms
    std::vector<double> value = {};

    if (filter.metric == MetricType::AVG_PACKAGE_TIME_IN_SYSTEM)
    {
        for (auto& ex : m_avgPacketTimeInSystem)
        {
            time.emplace_back((double)ex.first / g_oneMillisecond);
            value.emplace_back((double)ex.second / g_oneMillisecond);
        }
        return {time, value};
    }

    if (filter.addr == -1)
    {

        if (filter.roles == std::vector<RoleType>{RoleType::PROVIDER}) list = &m_avgProvider;
        else if (filter.roles == std::vector<RoleType>{RoleType::PRODUCER}) list = &m_avgProducer;
        else if (filter.roles == std::vector<RoleType>{RoleType::CONSUMER}) list = &m_avgConsumer;
        else if (filter.roles == std::vector<RoleType>{RoleType::PROVIDER, RoleType::PRODUCER}) list = &m_avgProviderProducer;
        else if (filter.roles == std::vector<RoleType>{RoleType::PROVIDER, RoleType::CONSUMER}) list = &m_avgProviderConsumer;
        else if (filter.roles == std::vector<RoleType>{RoleType::PRODUCER, RoleType::CONSUMER}) list = &m_avgProducerConsumer;
        else if (filter.roles == std::vector<RoleType>{RoleType::PROVIDER, RoleType::PRODUCER, RoleType::CONSUMER}) list = &m_avgProviderProducerConsumer;
        else if (filter.roles == std::vector<RoleType>{}) list = &m_avgAll;

        if (list == nullptr) return {};
        if (list->size() == 0) return {};
    }
    else
    {
        auto iter = m_nodeReports.find(filter.addr);
        if (iter == m_nodeReports.end()) return {};
        list = &iter->second;
        if (list->size() == 0) return {};
        if (list->front().second.roles != filter.roles) return {};
    }

    for (auto iter = list->begin(); iter != list->end(); iter++)
    {
        double metricVal = -1;
        switch (filter.metric)
        {
        case MetricType::SPEED :
        {
            metricVal = iter->second.speed;
            break;
        }
        case MetricType::PACKET_LOSS :
        {
            metricVal = iter->second.packetLoss;
            break;
        }
        case MetricType::PING :
        {
            metricVal = iter->second.ping;
            break;
        }
        case MetricType::AVG_PACKAGE_TIME_IN_SYSTEM :
        {
            break;
        }
        }

        time.emplace_back((double)iter->first / g_oneMillisecond);
        value.emplace_back(metricVal);
    }

    return {time, value};

}






















