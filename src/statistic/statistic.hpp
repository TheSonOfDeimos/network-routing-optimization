#ifndef STATISTIC_HPP
#define STATISTIC_HPP

#include <thread>
#include <unordered_map>
#include <map>
#include <deque>

#include "types.hpp"
#include "node.hpp"
#include "time.hpp"
#include "package.hpp"


enum class MetricType : int
{
    SPEED = 0,
    PACKET_LOSS,
    PING
};

struct Filter
{
    std::vector<RoleType> roles;
    MetricType metric;
    hostAddress_t addr = -1; // -1 for all nodes
};

struct Record
{
    hostAddress_t addr;
    double ping; // ms
    double packetLoss; // amount of lost packets in % per 100 packets
    double speed; // Mbit/sec
    std::vector<RoleType> roles;
};

using record_t = std::pair<modelTime_t, Record>;
using recordsList_t = std::deque<record_t>;
using subscriptionCallback = std::function<void(modelTime_t, double)>; // <time, metric value>
using addAllDataCallback = std::function<void(const std::vector<double>&, const std::vector<double>&)>; // <time, metric value>
using subId = int;

class Statistic
{
public:
    Statistic() = default;
    Statistic(const Statistic&) = delete;
    Statistic& operator = (Statistic&) = delete;

    status_t report(const NodeCharacteristics& params);
    status_t report(packagePtr_t package);

    status_t subscribe(subId id, subscriptionCallback call, addAllDataCallback addAll, const Filter& filter);
    status_t unsubscribe(subId id);

private:
    status_t onNodeReport(const record_t& rec);
    bool isReportSameSize();

    record_t countLastAverage(const std::vector<RoleType>& roles);

    recordsList_t *get(const Filter& filter);

private:
    std::unordered_map<hostAddress_t, recordsList_t> m_nodeReports;

    recordsList_t m_avgProvider;
    recordsList_t m_avgProducer;
    recordsList_t m_avgConsumer;

    recordsList_t m_avgProviderProducer;
    recordsList_t m_avgProviderConsumer;
    recordsList_t m_avgProducerConsumer;

    recordsList_t m_avgProviderProducerConsumer;
    recordsList_t m_avgAll;

    std::list<packagePtr_t> m_packagesBin;

    std::map<subId, std::pair<subscriptionCallback, Filter>> m_subscribers;

    std::mutex m_mtx;


};

#endif
