#include <iostream>

#include <QApplication>

#include "model/modelCore.hpp"
#include "gui/mainwindow.hpp"

int main(int argc, char *argv[])
{

    status_t status = ERROR_OK;

    int switchCount = 4;
    int maxPathLength = 6;
    TopologyType topologyType = TopologyType::D_CELL;
    AlgorithmType algorithmType = AlgorithmType::BELLMAN_FORD;
    NodeCharacteristics serverParams;
    NodeCharacteristics switchParams;

    serverParams.roles.push_back(RoleType::PRODUCER);
    serverParams.roles.push_back(RoleType::CONSUMER);
    serverParams.packageProduceFrequency = 1000000;
    serverParams.minPackageSize = 1500;
    serverParams.maxPackageSize = 2000;
    
    auto model = std::make_shared<ModelCore>();
    QApplication a(argc, argv);
    MainWindow w(model);

    RUN(model->buildNetworkTopology(topologyType, switchCount, serverParams, switchParams));
    RUN(model->buildRoutingTable(algorithmType, maxPathLength));


    w.show();
    status = a.exec();
    
    
exit:
    return status;
}
