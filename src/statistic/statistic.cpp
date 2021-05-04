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



status_t Statistic::subscribe(subId id, subscriptionCallback call, addAllDataCallback addAll, const Filter &filter)
{
    status_t status = ERROR_OK;

    std::lock_guard<std::mutex> lock(m_mtx);
    m_subscribers[id] = {call, filter};

    auto listToSend = get(filter);
    std::vector<double> time;
    std::vector<double> value;
    EXIT_IF(listToSend == nullptr, ERROR_OK);

    for (auto iter = listToSend->begin(); iter != listToSend->end(); iter++)
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
        }

        time.emplace_back((double)iter->first / g_oneMillisecond);
        value.emplace_back(metricVal);
    }

    addAll(time, value);


exit:
    return status;
}

status_t Statistic::unsubscribe(subId id)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    m_subscribers.erase(id);
    return ERROR_OK;
}

status_t Statistic::onNodeReport(const record_t &rec)
{
    status_t status = ERROR_OK;

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
        if (avgProviderConsumer.second.speed >= 0) m_avgProviderConsumer.push_back(avgProviderConsumer);

        auto avgProducerConsumer = countLastAverage({RoleType::PRODUCER, RoleType::CONSUMER});
        if (avgProducerConsumer.second.speed >= 0) m_avgProducerConsumer.push_back(avgProducerConsumer);

        auto avgProviderProducerConsumer = countLastAverage({RoleType::PROVIDER, RoleType::PRODUCER, RoleType::CONSUMER});
        if (avgProviderProducerConsumer.second.speed >= 0) m_avgProviderProducerConsumer.push_back(avgProviderProducerConsumer);

        auto avgAll = countLastAverage({});
        if (avgAll.second.speed >= 0) m_avgAll.push_back(avgAll);

        for (auto& sub : m_subscribers)
        {
            if (sub.second.second.addr != -1) continue;

            record_t* recToSend = nullptr;
            if (sub.second.second.roles == std::vector<RoleType>{RoleType::PROVIDER}) recToSend = &avgProvider;
            else if (sub.second.second.roles == std::vector<RoleType>{RoleType::PRODUCER}) recToSend = &avgProducer;
            else if (sub.second.second.roles == std::vector<RoleType>{RoleType::CONSUMER}) recToSend = &avgConsumer;
            else if (sub.second.second.roles == std::vector<RoleType>{RoleType::PROVIDER, RoleType::PRODUCER}) recToSend = &avgProviderProducer;
            else if (sub.second.second.roles == std::vector<RoleType>{RoleType::PROVIDER, RoleType::CONSUMER}) recToSend = &avgProviderConsumer;
            else if (sub.second.second.roles == std::vector<RoleType>{RoleType::PRODUCER, RoleType::CONSUMER}) recToSend = &avgProducerConsumer;
            else if (sub.second.second.roles == std::vector<RoleType>{RoleType::PROVIDER, RoleType::PRODUCER, RoleType::CONSUMER}) recToSend = &avgProviderProducerConsumer;
            else if (sub.second.second.roles == std::vector<RoleType>{}) recToSend = &avgAll;

            if (recToSend == nullptr) continue;
            if (recToSend->second.speed < 0) continue;

            double metricVal = -1;
            switch (sub.second.second.metric)
            {
            case MetricType::SPEED :
            {
                metricVal = recToSend->second.speed;
                break;
            }
            case MetricType::PACKET_LOSS :
            {
                metricVal = recToSend->second.packetLoss;
                break;
            }
            case MetricType::PING :
            {
                metricVal = recToSend->second.ping;
                break;
            }
            }

            sub.second.first(rec.first, metricVal);

        }
    }

    for (auto& sub : m_subscribers)
    {
        if (sub.second.second.addr == -1) continue;
        if (sub.second.second.addr != rec.second.addr) continue;
        if (sub.second.second.roles != rec.second.roles) continue;

        double metricVal = -1;
        switch (sub.second.second.metric)
        {
        case MetricType::SPEED :
        {
            metricVal = rec.second.speed;
            break;
        }
        case MetricType::PACKET_LOSS :
        {
            metricVal = rec.second.packetLoss;
            break;
        }
        case MetricType::PING :
        {
            metricVal = rec.second.ping;
            break;
        }
        }

        sub.second.first(rec.first, metricVal);
    }

exit:
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

recordsList_t* Statistic::get(const Filter &filter)
{
    if (filter.addr == -1)
    {
        recordsList_t* list = nullptr;
        if (filter.roles == std::vector<RoleType>{RoleType::PROVIDER}) list = &m_avgProvider;
        else if (filter.roles == std::vector<RoleType>{RoleType::PRODUCER}) list = &m_avgProducer;
        else if (filter.roles == std::vector<RoleType>{RoleType::CONSUMER}) list = &m_avgConsumer;
        else if (filter.roles == std::vector<RoleType>{RoleType::PROVIDER, RoleType::PRODUCER}) list = &m_avgProviderProducer;
        else if (filter.roles == std::vector<RoleType>{RoleType::PROVIDER, RoleType::CONSUMER}) list = &m_avgProviderConsumer;
        else if (filter.roles == std::vector<RoleType>{RoleType::PRODUCER, RoleType::CONSUMER}) list = &m_avgProducerConsumer;
        else if (filter.roles == std::vector<RoleType>{RoleType::PROVIDER, RoleType::PRODUCER, RoleType::CONSUMER}) list = &m_avgProviderProducerConsumer;
        else if (filter.roles == std::vector<RoleType>{}) list = &m_avgAll;

        if (list == nullptr) return nullptr;
        if (list->size() == 0) return nullptr;
        return list;
    }
    else
    {
        recordsList_t* list = nullptr;

        auto iter = m_nodeReports.find(filter.addr);
        if (iter == m_nodeReports.end()) return nullptr;
        list = &iter->second;
        if (list->size() == 0) return nullptr;
        if (list->front().second.roles != filter.roles) return nullptr;
        return list;
    }
}






















