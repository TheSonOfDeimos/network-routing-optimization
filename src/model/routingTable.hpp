#ifndef ROUTING_TABLE_HPP
#define ROUTING_TABLE_HPP

#include <vector>
#include <mutex>
#include <memory>
#include <map>

#include "types.hpp"
#include "node.hpp"
#include "bellmanFord.hpp"
#include "deimosV1.hpp"
#include "dijikstra.hpp"

enum class AlgorithmType : int
{
    DEIMOS_V1 = 0,
    DIJKSTRA,
    BELLMAN_FORD
};

// Thread-safe
class RoutingTable
{
public:
    RoutingTable() = default;

    Route get(hostAddress_t src, hostAddress_t dest) const;
    status_t buildRoutes(AlgorithmType type, const std::map<hostAddress_t, std::shared_ptr<Node>>& nodes,
                         int maxPathLength, double reqSpeed, double reqPacketloss, double reqPing);

private:
    ConnectMatrix_t hostsToMatrix(const std::map<hostAddress_t, std::shared_ptr<Node>> & nodes);

private:
    RouteTable_t m_table; // sorted by cost with the same source
    std::unique_ptr<AlgorithmBase> m_algo;
    std::mutex m_mtx;
};

#endif
