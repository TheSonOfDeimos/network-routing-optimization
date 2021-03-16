#include <iostream>

#include "core/time.hpp"
#include "core/core.hpp"
#include "model/node.hpp"
#include "model/routingTable.hpp"
#include "model/modelCore.hpp"
#include "model/packageQueue.hpp"

#include "algorithm/bellmanFord.hpp"

int main()
{

    status_t status = ERROR_OK;

    auto table = std::make_shared<RoutingTable>();
    std::map<hostAddress_t, std::shared_ptr<Node> > nodeMap;

    auto node_1 = std::make_shared<Node>(NodeCharacteristics{}, table);
    nodeMap.insert({node_1->getNodeCharacteristics().addr, node_1});

    auto node_2 = std::make_shared<Node>(NodeCharacteristics{}, table);
    nodeMap.insert({node_2->getNodeCharacteristics().addr, node_2});

    auto node_3 = std::make_shared<Node>(NodeCharacteristics{}, table);
    nodeMap.insert({node_3->getNodeCharacteristics().addr, node_3});

    auto node_4 = std::make_shared<Node>(NodeCharacteristics{}, table);
    nodeMap.insert({node_4->getNodeCharacteristics().addr, node_4});

    auto node_5 = std::make_shared<Node>(NodeCharacteristics{}, table);
    nodeMap.insert({node_5->getNodeCharacteristics().addr, node_5});

    RUN(node_1->connectToNode(node_2));
    RUN(node_1->connectToNode(node_3));
    RUN(node_2->connectToNode(node_4));
    RUN(node_2->connectToNode(node_3));
    RUN(node_4->connectToNode(node_5));

    RUN(table->buildRoutes(AlgorithmType::BELLMAN_FORD, nodeMap));

exit:
    return status;
}