#include "routingTable.hpp"

#include <algorithm>
#include <memory>

#include "bellmanFord.hpp"

Route RoutingTable::get(hostAddress_t src, hostAddress_t dest) const
{
    auto found = std::find_if(m_table.begin(), m_table.end(), [&src, &dest](auto &route) {
        if (route.source == src && route.destination == dest)
        {
            return true;
        }
        return false;
    });

    if (found == m_table.end())
    {
        return {};
    }
    return *found;
}

status_t RoutingTable::buildRoutes(AlgorithmType type, const std::map<hostAddress_t, std::shared_ptr<Node> > &nodes)
{
    status_t status = ERROR_OK;
    auto matrix = hostsToMatrix(nodes);

    std::unique_lock<std::mutex> lock(m_mtx);

    switch (type)
    {
    case AlgorithmType::SCRAT:
    {
        NOT_IMPLEMENTED_YET;
        break;
    }
    case AlgorithmType::DIJKSTRA:
    {
        NOT_IMPLEMENTED_YET;
        break;
    }
    case AlgorithmType::BELLMAN_FORD:
    {
        m_algo = std::make_unique<BellmanFord>();
        break;
    }
    default:
        EXIT(ERROR_NO_EFFECT);
        break;
    }

    m_table = m_algo->buildRoutes(matrix);
    lock.unlock();

exit:
    return status;
}

ConnectMatrix_t RoutingTable::hostsToMatrix(const std::map<hostAddress_t, std::shared_ptr<Node> > &nodes)
{
    ConnectMatrix_t matrix;

    for (auto &node : nodes)
    {
        auto ch_1 = node.second->getNodeCharacteristics();
        Cell cell = {
            ch_1.ping,
            ch_1.packetLoss,
            ch_1.packetDrop,
            ch_1.speed,
            ch_1.bandwidth,
            ch_1.bufferVolume};

        matrix[ch_1.addr] = {};
        auto cons = node.second->getConnections();
        for (auto &con : cons)
        {
            auto ch_2 = con.second->getNodeCharacteristics();
            cell.path = {ch_1.addr, ch_2.addr};
            matrix.at(ch_1.addr)[ch_2.addr] = cell;
        }
    }

    return matrix;
}
