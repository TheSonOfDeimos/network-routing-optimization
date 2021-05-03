#include "topologyBuilder.hpp"

#include "statistic.hpp"

TopologyBuilder::nodeMap_t TopologyBuilder::build(TopologyType type, std::shared_ptr<RoutingTable> table, int switchCount, const NodeCharacteristics& serverParams, const NodeCharacteristics& switchParams, std::shared_ptr<Statistic> statistic)
{
    status_t status = ERROR_OK;

    nodeMap_t nodeMap;

    switch (type)
    {
    case TopologyType::D_CELL:
    {
        nodeMap = buildDCell(table, switchCount, serverParams, switchParams, statistic);
        break;
    }
    case TopologyType::B_CUBE:
    {
        NOT_IMPLEMENTED_YET;
        break;
    }
        case TopologyType::FAT_TREE:
    {
        NOT_IMPLEMENTED_YET;
        break;
    }
    case TopologyType::FREE:
    {
        NOT_IMPLEMENTED_YET;
        break;
    }

    default:
    {
        EXIT(ERROR_LOGIC);
    }
    }

exit:
    return nodeMap;
}

TopologyBuilder::nodeMap_t TopologyBuilder::buildDCell(std::shared_ptr<RoutingTable> table, int switchCount, const NodeCharacteristics& serverParams, const NodeCharacteristics& switchParams, std::shared_ptr<Statistic> statistic)
{
    using nodesVector_t = std::vector<std::shared_ptr<Node>>;
    using nodesCellsVector_t = std::vector<nodesVector_t>;

    status_t status = ERROR_OK;

    int nodesPerCell = switchCount - 1;
    nodesVector_t switchNodeVec;
    nodesCellsVector_t computingNodesCellsVec;

    nodeMap_t nodeMap;

    for (int i = 0; i < switchCount; i++)
    {
        auto node = std::make_shared<Node>(switchParams, table, statistic);
        nodeMap.insert({node->getNodeCharacteristics().addr, node});
        switchNodeVec.push_back(node);
    }

    for (int s = 0; s < switchCount; s++)
    {
        computingNodesCellsVec.push_back({});
        for (int i = 0; i < nodesPerCell; i++)
        {
            auto node = std::make_shared<Node>(serverParams, table, statistic);
            nodeMap.insert({node->getNodeCharacteristics().addr, node});
            computingNodesCellsVec.at(s).push_back(node);
        }
    }

    // Connection
    for (int s = 0; s < switchCount; s++)
    {
        computingNodesCellsVec.push_back({});
        for (int i = s; i < nodesPerCell; i++)
        {
            computingNodesCellsVec.at(s).at(i)->connectToNode(computingNodesCellsVec.at(i + 1).at(s));
            
        }

        for (int i = 0; i < nodesPerCell; i++)
        {
            computingNodesCellsVec.at(s).at(i)->connectToNode(switchNodeVec.at(s));
        }

    }

exit:
    return nodeMap;
}
