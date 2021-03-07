#ifndef ROUTING_TABLE_HPP
#define ROUTING_TABLE_HPP

#include <vector>

#include "types.hpp"

struct Route
{
    double cost;
    std::vector<std::pair<hostId_t, hostAddress_t>> path;
    std::pair<hostId_t, hostAddress_t> source;
    std::pair<hostId_t, hostAddress_t> destination;
};

class RoutingTable
{
public:
    const Route& get(hostAddress_t src, hostAddress_t dest);
    status_t add(const Route& route);


private:
    std::vector<Route> m_table; // sorted by cost with the same source


};

#endif