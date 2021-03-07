#include "routingTable.hpp"

#include <algorithm>

const Route& RoutingTable::get(hostAddress_t src, hostAddress_t dest)
{
    auto r = std::find_if(m_table.begin(), m_table.end(), [&dest](Route& route)
    {
        if (route.destination.second == dest)
        {
            return true;
        }

        return false;
    });

    return *r;

}

status_t RoutingTable::add(const Route &route)
{
    m_table.push_back(route);
    return 0;
}