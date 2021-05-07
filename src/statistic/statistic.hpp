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
    PING,
    AVG_PACKAGE_TIME_IN_SYSTEM
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
using subId = int;

class Statistic
{
public:
    Statistic() = default;
    Statistic(const Statistic&) = delete;
    Statistic& operator = (Statistic&) = delete;

    status_t report(const NodeCharacteristics& params);
    status_t report(packagePtr_t package);
    std::pair<std::vector<double>, std::vector<double> > get(const Filter& filter);

private:
    status_t onNodeReport(const record_t& rec);
    status_t onPacketReport(const Package& package);
    bool isReportSameSize();
    record_t countLastAverage(const std::vector<RoleType>& roles);

private:
    // Node stat
    std::unordered_map<hostAddress_t, recordsList_t> m_nodeReports;
    recordsList_t m_avgProvider;
    recordsList_t m_avgProducer;
    recordsList_t m_avgConsumer;
    recordsList_t m_avgProviderProducer;
    recordsList_t m_avgProviderConsumer;
    recordsList_t m_avgProducerConsumer;
    recordsList_t m_avgProviderProducerConsumer;
    recordsList_t m_avgAll;

    // Packets stat
    std::list<packagePtr_t> m_packagesBin;
    std::vector<modelTime_t> m_measurePacketTimeInSystem;
    int m_measureFrame = 200;
    std::vector<std::pair<modelTime_t, modelTime_t>> m_avgPacketTimeInSystem;

    std::mutex m_mtx;


};

#endif
