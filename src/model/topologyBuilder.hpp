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

    static nodeMap_t build(TopologyType type, std::shared_ptr<RoutingTable> table, int switchCount, const NodeCharacteristics& serverParams, const NodeCharacteristics& switchParams, std::shared_ptr<Statistic> statisitc);

private:
    static nodeMap_t buildDCell(std::shared_ptr<RoutingTable> table, int switchCount, const NodeCharacteristics& serverParams, const NodeCharacteristics& switchParams, std::shared_ptr<Statistic> statistic);
    
};

#endif
