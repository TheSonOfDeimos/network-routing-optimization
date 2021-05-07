#ifndef TOPOLOGY_BUILDER_HPP
#define TOPOLOGY_BUILDER_HPP

#include "node.hpp"
#include "routingTable.hpp"

class Statistic;

enum class TopologyType : int
{
    D_CELL = 0,
    B_CUBE,
    FAT_TREE,
    FREE
};

class TopologyBuilder
{
public:
    using nodeMap_t = std::map<hostAddress_t, std::shared_ptr<Node>>;

    static nodeMap_t build(TopologyType type, std::shared_ptr<RoutingTable> table, int switchCount,
                           NodeCharacteristics serverParams, NodeCharacteristics switchParams, NodeCharacteristics borderGatewayParams,
                           int partOfAnomalyServers, int partOfAnomalySwitches, int partOfAnomalyBorderGateways,
                           NodeCharacteristics anomalyServerParams, NodeCharacteristics anomalySwitchParams, NodeCharacteristics anomalyBorderGatewayParams,
                           std::shared_ptr<Statistic> statistic);

private:
    static nodeMap_t buildDCell(std::shared_ptr<RoutingTable> table, int switchCount,
                                NodeCharacteristics& serverParams, NodeCharacteristics& switchParams,
                                int partOfAnomalyServers, int partOfAnomalySwitches,
                                NodeCharacteristics anomalyServerParams, NodeCharacteristics anomalySwitchParams,
                                std::shared_ptr<Statistic> statistic);

    static nodeMap_t buildFatTree(std::shared_ptr<RoutingTable> table, int switchCount,
                                  NodeCharacteristics serverParams, NodeCharacteristics switchParams, NodeCharacteristics borderGatewayParams,
                                  int partOfAnomalyServers, int partOfAnomalySwitches, int partOfAnomalyBorderGateways,
                                  NodeCharacteristics anomalyServerParams, NodeCharacteristics anomalySwitchParams, NodeCharacteristics anomalyBorderGatewayParams,
                                  std::shared_ptr<Statistic> statistic);

    static nodeMap_t buildBCube(std::shared_ptr<RoutingTable> table, int switchCount,
                                NodeCharacteristics serverParams, NodeCharacteristics switchParams, NodeCharacteristics borderGatewayParams,
                                int partOfAnomalyServers, int partOfAnomalySwitches, int partOfAnomalyBorderGateways,
                                NodeCharacteristics anomalyServerParams, NodeCharacteristics anomalySwitchParams, NodeCharacteristics anomalyBorderGatewayParams,
                                std::shared_ptr<Statistic> statistic);

};

#endif
