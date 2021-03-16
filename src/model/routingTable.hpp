#ifndef ROUTING_TABLE_HPP
#define ROUTING_TABLE_HPP

#include <vector>
#include <mutex>
#include <memory>
#include <map>

#include "types.hpp"
#include "algorithmBase.hpp"
#include "node.hpp"

enum class AlgorithmType : int
{
    SCRAT = 0,
    DIJKSTRA,
    BELLMAN_FORD
};

class RoutingTable
{
public:
    RoutingTable() = default;

    Route get(hostAddress_t src, hostAddress_t dest) const;
    status_t buildRoutes(AlgorithmType type, const std::map<hostAddress_t, std::shared_ptr<Node>>& nodes);

private:
    ConnectMatrix_t hostsToMatrix(const std::map<hostAddress_t, std::shared_ptr<Node>> & nodes);

private:
    RouteTable_t m_table; // sorted by cost with the same source
    std::unique_ptr<AlgorithmBase> m_algo;
    std::mutex m_mtx;
};

#endif