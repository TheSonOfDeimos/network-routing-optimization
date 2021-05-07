#include "topologyBuilder.hpp"

#include <algorithm>
#include <random>

#include "statistic.hpp"

TopologyBuilder::nodeMap_t TopologyBuilder::build(TopologyType type, std::shared_ptr<RoutingTable> table, int switchCount,
                                                  NodeCharacteristics serverParams, NodeCharacteristics switchParams, NodeCharacteristics borderGatewayParams,
                                                  int partOfAnomalyServers, int partOfAnomalySwitches, int partOfAnomalyBorderGateways,
                                                  NodeCharacteristics anomalyServerParams, NodeCharacteristics anomalySwitchParams, NodeCharacteristics anomalyBorderGatewayParams,
                                                  std::shared_ptr<Statistic> statistic)
{
    status_t status = ERROR_OK;

    nodeMap_t nodeMap;

    switch (type)
    {
    case TopologyType::D_CELL:
    {
        nodeMap = buildDCell(table, switchCount, serverParams, switchParams, partOfAnomalyServers, partOfAnomalySwitches, anomalyServerParams, anomalySwitchParams, statistic);
        break;
    }
    case TopologyType::B_CUBE:
    {
        nodeMap = buildBCube(table, switchCount, serverParams, switchParams, borderGatewayParams, partOfAnomalyServers, partOfAnomalySwitches, partOfAnomalyBorderGateways, anomalyServerParams, anomalySwitchParams, anomalyBorderGatewayParams, statistic);
        break;
    }
        case TopologyType::FAT_TREE:
    {
        nodeMap = buildFatTree(table, switchCount, serverParams, switchParams, borderGatewayParams, partOfAnomalyServers, partOfAnomalySwitches, partOfAnomalyBorderGateways, anomalyServerParams, anomalySwitchParams, anomalyBorderGatewayParams, statistic);
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

TopologyBuilder::nodeMap_t TopologyBuilder::buildDCell(std::shared_ptr<RoutingTable> table, int switchCount,
                                                       NodeCharacteristics& serverParams, NodeCharacteristics& switchParams,
                                                       int partOfAnomalyServers, int partOfAnomalySwitches,
                                                       NodeCharacteristics anomalyServerParams, NodeCharacteristics anomalySwitchParams,
                                                       std::shared_ptr<Statistic> statistic)
{
    using nodesVector_t = std::vector<std::shared_ptr<Node>>;
    using nodesCellsVector_t = std::vector<nodesVector_t>;

    status_t status = ERROR_OK;

    int serversPerCellCount = switchCount - 1;
    int anomalyServersCount = serversPerCellCount * switchCount * ((double)partOfAnomalyServers / 100);
    int anomalySwitchCount = switchCount * ((double)partOfAnomalySwitches / 100);

    nodesVector_t switchNodeVec;
    nodesCellsVector_t computingNodesCellsVec;
    nodeMap_t nodeMap;

    for (int i = 0; i < switchCount; i++)
    {
        if (anomalySwitchCount != 0)
        {
            auto anomalySwitchNode = std::make_shared<Node>(anomalySwitchParams, table, statistic);
            nodeMap.insert({anomalySwitchNode->getNodeCharacteristics().addr, anomalySwitchNode});
            switchNodeVec.push_back(anomalySwitchNode);
            anomalySwitchCount--;
        }
        else
        {
            auto switchNode = std::make_shared<Node>(switchParams, table, statistic);
            nodeMap.insert({switchNode->getNodeCharacteristics().addr, switchNode});
            switchNodeVec.push_back(switchNode);
        }
    }

    for (int s = 0; s < switchCount; s++)
    {
        computingNodesCellsVec.push_back({});
        for (int i = 0; i < serversPerCellCount; i++)
        {
            if (anomalyServersCount != 0)
            {
                auto anomalyServerNode = std::make_shared<Node>(anomalyServerParams, table, statistic);
                nodeMap.insert({anomalyServerNode->getNodeCharacteristics().addr, anomalyServerNode});
                computingNodesCellsVec.at(s).push_back(anomalyServerNode);

                anomalyServersCount--;
            }
            else
            {
                auto serverNode = std::make_shared<Node>(serverParams, table, statistic);
                nodeMap.insert({serverNode->getNodeCharacteristics().addr, serverNode});
                computingNodesCellsVec.at(s).push_back(serverNode);
            }
        }
    }

    // Connection
    for (int s = 0; s < switchCount; s++)
    {
        computingNodesCellsVec.push_back({});
        for (int i = s; i < serversPerCellCount; i++)
        {
            computingNodesCellsVec.at(s).at(i)->connectToNode(computingNodesCellsVec.at(i + 1).at(s));
            
        }

        for (int i = 0; i < serversPerCellCount; i++)
        {
            computingNodesCellsVec.at(s).at(i)->connectToNode(switchNodeVec.at(s));
        }

    }

exit:
    return nodeMap;
}

TopologyBuilder::nodeMap_t TopologyBuilder::buildFatTree(std::shared_ptr<RoutingTable> table, int switchCount,
                                                         NodeCharacteristics serverParams, NodeCharacteristics switchParams, NodeCharacteristics borderGatewayParams,
                                                         int partOfAnomalyServers, int partOfAnomalySwitches, int partOfAnomalyBorderGateways,
                                                         NodeCharacteristics anomalyServerParams, NodeCharacteristics anomalySwitchParams, NodeCharacteristics anomalyBorderGatewayParams,
                                                         std::shared_ptr<Statistic> statistic)
{
    status_t status = ERROR_OK;

    using nodesVector_t = std::vector<std::shared_ptr<Node>>;

    int borderGatewayCount = switchCount;
    int switchNodeCount = borderGatewayCount * 4;
    int serverCount = switchNodeCount;

    int anomalyBorderGatewayCount = borderGatewayCount * ((double)partOfAnomalyBorderGateways / 100);
    int anomalySwitchNodeCount = switchNodeCount * ((double)partOfAnomalySwitches / 100);
    int anomalyServerCount = serverCount * ((double)partOfAnomalyServers / 100);

    nodeMap_t nodeMap;
    nodesVector_t borderGatewayNodes = {};
    nodesVector_t switchNodes = {};
    nodesVector_t serverNodes = {};

    for (int i = 0; i < borderGatewayCount; i++)
    {
        if (anomalyBorderGatewayCount != 0)
        {
            auto anomalyBorderGatewayNode = std::make_shared<Node>(anomalyBorderGatewayParams, table, statistic);
            nodeMap.insert({anomalyBorderGatewayNode->getNodeCharacteristics().addr, anomalyBorderGatewayNode});
            borderGatewayNodes.push_back(anomalyBorderGatewayNode);

            anomalyBorderGatewayCount--;
        }
        else
        {
            auto borderGatewayNode = std::make_shared<Node>(borderGatewayParams, table, statistic);
            nodeMap.insert({borderGatewayNode->getNodeCharacteristics().addr, borderGatewayNode});
            borderGatewayNodes.push_back(borderGatewayNode);
        }
    }

    for (int i = 0; i < switchNodeCount; i++)
    {
        if (anomalySwitchNodeCount != 0 && i % 4 == 0)
        {
            auto anomalySwitchNode = std::make_shared<Node>(anomalySwitchParams, table, statistic);
            nodeMap.insert({anomalySwitchNode->getNodeCharacteristics().addr, anomalySwitchNode});
            switchNodes.push_back(anomalySwitchNode);

            anomalySwitchNodeCount--;
        }
        else
        {
            auto anomalySwitchNode = std::make_shared<Node>(switchParams, table, statistic);
            nodeMap.insert({anomalySwitchNode->getNodeCharacteristics().addr, anomalySwitchNode});
            switchNodes.push_back(anomalySwitchNode);
        }
    }

    for (int i = 0; i < serverCount; i++)
    {
        if (anomalyServerCount != 0)
        {
            auto anomalyServerNode = std::make_shared<Node>(anomalyServerParams, table, statistic);
            nodeMap.insert({anomalyServerNode->getNodeCharacteristics().addr, anomalyServerNode});
            serverNodes.push_back(anomalyServerNode);

            anomalyServerCount--;
        }
        else
        {
            auto serverNode = std::make_shared<Node>(serverParams, table, statistic);
            nodeMap.insert({serverNode->getNodeCharacteristics().addr, serverNode});
            serverNodes.push_back(serverNode);
        }
    }

    // Connection
    // Connect Border gateway to L1 swotch
    for (int bg = 0; bg < borderGatewayCount; bg++)
    {
        for (int i = 0; i < borderGatewayCount; i++)
        {
            int part = bg >= borderGatewayCount / 2 ? 1 : 0;
            int switchNodeIndex = i * 4 + part;
            borderGatewayNodes.at(bg)->connectToNode(switchNodes.at(switchNodeIndex));
        }
    }

    // Connect Sitch L1 to Switch L2 and Servers to Switch L2
    for (int sw = 0; sw < switchNodeCount; sw += 4)
    {
        // Connect L1 to L2
        switchNodes.at(sw)->connectToNode(switchNodes.at(sw + 2));
        switchNodes.at(sw)->connectToNode(switchNodes.at(sw + 3));
        switchNodes.at(sw + 1)->connectToNode(switchNodes.at(sw + 2));
        switchNodes.at(sw + 1)->connectToNode(switchNodes.at(sw + 3));

        // Connect Servers
        switchNodes.at(sw + 2)->connectToNode(serverNodes.at(sw));
        switchNodes.at(sw + 2)->connectToNode(serverNodes.at(sw + 1));
        switchNodes.at(sw + 3)->connectToNode(serverNodes.at(sw + 2));
        switchNodes.at(sw + 3)->connectToNode(serverNodes.at(sw + 3));
    }

exit:
    return nodeMap;
}

TopologyBuilder::nodeMap_t TopologyBuilder::buildBCube(std::shared_ptr<RoutingTable> table, int switchCount,
                                                       NodeCharacteristics serverParams, NodeCharacteristics switchParams, NodeCharacteristics borderGatewayParams,
                                                       int partOfAnomalyServers, int partOfAnomalySwitches, int partOfAnomalyBorderGateways,
                                                       NodeCharacteristics anomalyServerParams, NodeCharacteristics anomalySwitchParams, NodeCharacteristics anomalyBorderGatewayParams,
                                                       std::shared_ptr<Statistic> statistic)
{
    status_t status = ERROR_OK;

    using nodesVector_t = std::vector<std::shared_ptr<Node>>;

    int borderGatewayCount = switchCount;
    int switchNodeCount = borderGatewayCount;
    int serverCount = switchNodeCount * switchNodeCount;

    int anomalyBorderGatewayCount = borderGatewayCount * ((double)partOfAnomalyBorderGateways / 100);
    int anomalySwitchNodeCount = switchNodeCount * ((double)partOfAnomalySwitches / 100);
    int anomalyServerCount = serverCount * ((double)partOfAnomalyServers / 100);

    nodeMap_t nodeMap;
    nodesVector_t borderGatewayNodes = {};
    nodesVector_t switchNodes = {};
    nodesVector_t serverNodes = {};

    for (int i = 0; i < borderGatewayCount; i++)
    {
        if (anomalyBorderGatewayCount != 0)
        {
            auto anomalyBorderGatewayNode = std::make_shared<Node>(anomalyBorderGatewayParams, table, statistic);
            nodeMap.insert({anomalyBorderGatewayNode->getNodeCharacteristics().addr, anomalyBorderGatewayNode});
            borderGatewayNodes.push_back(anomalyBorderGatewayNode);

            anomalyBorderGatewayCount--;
        }
        else
        {
            auto borderGatewayNode = std::make_shared<Node>(borderGatewayParams, table, statistic);
            nodeMap.insert({borderGatewayNode->getNodeCharacteristics().addr, borderGatewayNode});
            borderGatewayNodes.push_back(borderGatewayNode);
        }
    }

    for (int i = 0; i < switchNodeCount; i++)
    {
        if (anomalySwitchNodeCount != 0)
        {
            auto anomalySwitchNode = std::make_shared<Node>(anomalySwitchParams, table, statistic);
            nodeMap.insert({anomalySwitchNode->getNodeCharacteristics().addr, anomalySwitchNode});
            switchNodes.push_back(anomalySwitchNode);

            anomalySwitchNodeCount--;
        }
        else
        {
            auto anomalySwitchNode = std::make_shared<Node>(switchParams, table, statistic);
            nodeMap.insert({anomalySwitchNode->getNodeCharacteristics().addr, anomalySwitchNode});
            switchNodes.push_back(anomalySwitchNode);
        }
    }

    for (int i = 0; i < serverCount; i++)
    {
        if (anomalyServerCount != 0)
        {
            auto anomalyServerNode = std::make_shared<Node>(anomalyServerParams, table, statistic);
            nodeMap.insert({anomalyServerNode->getNodeCharacteristics().addr, anomalyServerNode});
            serverNodes.push_back(anomalyServerNode);

            anomalyServerCount--;
        }
        else
        {
            auto serverNode = std::make_shared<Node>(serverParams, table, statistic);
            nodeMap.insert({serverNode->getNodeCharacteristics().addr, serverNode});
            serverNodes.push_back(serverNode);
        }
    }

    // Connection
    for (int sw = 0; sw < switchNodeCount; sw++)
    {
        int serverNodeIndex = sw * switchNodeCount;
        for (int i = 0; i < switchNodeCount; i++)
        {
            switchNodes.at(sw)->connectToNode(serverNodes.at(serverNodeIndex + i));
        }
    }

    for (int bg = 0; bg < borderGatewayCount; bg++)
    {
        for (int i = 0; i < borderGatewayCount; i++)
        {
            borderGatewayNodes.at(bg)->connectToNode(serverNodes.at(i * switchNodeCount + bg));
        }
    }

exit:
    return nodeMap;
}
