#include <iostream>

#include "model/modelCore.hpp"

int main()
{

    status_t status = ERROR_OK;

    int switchCount = 5;
    int maxPathLength = 6;
    TopologyType topologyType = TopologyType::D_CELL;
    AlgorithmType algorithmType = AlgorithmType::BELLMAN_FORD;
    NodeCharacteristics serverParams;
    NodeCharacteristics switchParams;

    serverParams.roles.push_back(RoleType::PRODUCER);
    serverParams.roles.push_back(RoleType::CONSUMER);
    serverParams.packageProduceFrequency = 1000000;
    serverParams.minPackageSize = 100;
    serverParams.maxPackageSize = 1000;
    
    ModelCore model;

    RUN(model.buildNetworkTopology(topologyType, switchCount, serverParams, switchParams));
    RUN(model.buildRoutingTable(algorithmType, maxPathLength));

    RUN(model.start(g_oneSecond / 10000));

    while (true)
    {
    }
    
    
exit:
    return status;
}
