#ifndef STATISTIC_HPP
#define STATISTIC_HPP

#include <thread>
#include <unordered_map>
#include <list>

#include "types.hpp"
#include "node.hpp"
#include "time.hpp"
#include "package.hpp"

using record_t = std::pair<modelTime_t, NodeCharacteristics>;
using recordsList_t = std::list<record_t>;

class Statistic
{
public:
    Statistic() = default;
    Statistic(const Statistic&) = delete;
    Statistic& operator = (Statistic&) = delete;

    static Statistic &instance();

    status_t report(const NodeCharacteristics& params);
    status_t report(packagePtr_t package);

private:

    std::unordered_map<hostAddress_t, recordsList_t> m_nodeReports;
    recordsList_t m_averageReports;

    std::mutex m_mtx;
};

#endif
