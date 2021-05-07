#include <iostream>

#include <QApplication>

#include "model/modelCore.hpp"
#include "gui/mainwindow.hpp"

int main(int argc, char *argv[])
{
    status_t status = ERROR_OK;

// =========================== FAT_TREE Overloaded Switch =================================
//    // Requirments
//    int maxPathLength = 11;
//    double reqSpeed = 100;
//    double reqPacketloss = 3;
//    double reqPing = 1;

//    // Global sytem params
//    modelTime_t modelingTimeStep = 400 * g_oneNanosecond;
//    modelTime_t modelingDuration = 1 * g_oneSecond;
//    modelTime_t rebuildRoutingTableEvery = 1 * g_oneMillisecond;
//    bool rebuildRoutingTableGradient = false;
//    int switchCount = 10;
//    TopologyType topologyType = TopologyType::FAT_TREE;
//    AlgorithmType algorithmType = AlgorithmType::DIJKSTRA;

//    // Node params
//    NodeCharacteristics serverParams;
//    serverParams.roles = {RoleType::PROVIDER, RoleType::CONSUMER};
//    serverParams.bandwidth = 5000;
//    serverParams.bufferVolume = 1;

//    NodeCharacteristics switchParams;
//    switchParams.roles = {RoleType::PROVIDER};
//    switchParams.bufferVolume = 1;
//    switchParams.bandwidth = 1000;

//    NodeCharacteristics borderGatewayParams;
//    borderGatewayParams.roles = {RoleType::PROVIDER, RoleType::PRODUCER};
//    borderGatewayParams.bandwidth = 10000;
//    borderGatewayParams.bufferVolume = 8;
//    borderGatewayParams.packageProduceFrequency = 150000;
//    borderGatewayParams.minPackageSize = 2000;
//    borderGatewayParams.maxPackageSize = 4000;

//    // Anomaly node params
//    int partOfAnomalyServers = 0; // %
//    int partOfAnomalySwitches = 5; // %
//    int partOfAnomalyBorderGateways = 0; // %

//    NodeCharacteristics anomalyServerParams;
//    NodeCharacteristics anomalySwitchParams;
//    anomalySwitchParams.NetworkInterfaceCount = 1;

//    NodeCharacteristics anomalyBorderGatewayParams;

    // =========================== FAT_TREE Overloaded Border =================================
    // Requirments
    int maxPathLength = 11;
    double reqSpeed = 50;
    double reqPacketloss = 10;
    double reqPing = 1;

    // Global sytem params
    modelTime_t modelingTimeStep = 400 * g_oneNanosecond;
    modelTime_t modelingDuration = 1 * g_oneSecond;
    modelTime_t rebuildRoutingTableEvery = 500 * g_oneMicrosecond;
    bool rebuildRoutingTableGradient = false;
    int switchCount = 10;
    TopologyType topologyType = TopologyType::FAT_TREE;
    AlgorithmType algorithmType = AlgorithmType::DIJKSTRA;

    // Node params
    NodeCharacteristics serverParams;
    serverParams.roles = {RoleType::PROVIDER, RoleType::CONSUMER};
    serverParams.bandwidth = 5000;
    serverParams.bufferVolume = 1;

    NodeCharacteristics switchParams;
    switchParams.roles = {RoleType::PROVIDER};
    switchParams.bufferVolume = 1;
    switchParams.NetworkInterfaceCount = 2;
    switchParams.bandwidth = 1000;

    NodeCharacteristics borderGatewayParams;
    borderGatewayParams.roles = {RoleType::PROVIDER, RoleType::PRODUCER};
    borderGatewayParams.bandwidth = 10000;
    borderGatewayParams.bufferVolume = 8;
    borderGatewayParams.packageProduceFrequency = 140000;
    borderGatewayParams.minPackageSize = 2000;
    borderGatewayParams.maxPackageSize = 4000;

    // Anomaly node params
    int partOfAnomalyServers = 0; // %
    int partOfAnomalySwitches = 0; // %
    int partOfAnomalyBorderGateways = 10; // %

    NodeCharacteristics anomalyServerParams;
    NodeCharacteristics anomalySwitchParams;
    NodeCharacteristics anomalyBorderGatewayParams;
    anomalyBorderGatewayParams.roles = {RoleType::PROVIDER, RoleType::PRODUCER};
    anomalyBorderGatewayParams.bandwidth = 10000;
    anomalyBorderGatewayParams.bufferVolume = 8;
    anomalyBorderGatewayParams.packageProduceFrequency = 200;
    anomalyBorderGatewayParams.minPackageSize = 880000;
    anomalyBorderGatewayParams.maxPackageSize = 900000;

    // =========================== D_CELL #1 Oveloaded Server =================================
    //    // Requirments
    //    int maxPathLength = 11;
    //    double reqSpeed = 10;
    //    double reqPacketloss = 30;
    //    double reqPing = 2;

    //    // Global sytem params
    //    modelTime_t modelingTimeStep = 400 * g_oneNanosecond;
    //    modelTime_t modelingDuration = 1 * g_oneSecond;
    //    modelTime_t rebuildRoutingTableEvery = 1 * g_oneMillisecond;
    //    bool rebuildRoutingTableGradient = false;
    //    int switchCount = 10;
    //    TopologyType topologyType = TopologyType::D_CELL;
    //    AlgorithmType algorithmType = AlgorithmType::DIJKSTRA;


    //    // Node params
    //    NodeCharacteristics serverParams;
    //    serverParams.roles = {RoleType::PROVIDER,  RoleType::PRODUCER, RoleType::CONSUMER};
    //    serverParams.bandwidth = 1000;
    //    serverParams.packageProduceFrequency = 100000;
    //    serverParams.minPackageSize = 1000;
    //    serverParams.maxPackageSize = 1500;

    //    NodeCharacteristics switchParams;
    //    switchParams.bandwidth = 10000;

    //    NodeCharacteristics borderGatewayParams;

    //    // Anomaly node params
    //    int partOfAnomalyServers = 7; // %
    //    int partOfAnomalySwitches = 0; // %
    //    int partOfAnomalyBorderGateways = 0; // %

    //    NodeCharacteristics anomalyServerParams;
    //    anomalyServerParams.roles = {RoleType::PROVIDER, RoleType::PRODUCER, RoleType::CONSUMER};
    //    anomalyServerParams.bandwidth = 1000;
    //    anomalyServerParams.packageProduceFrequency = 80000;
    //    anomalyServerParams.minPackageSize = 4000;
    //    anomalyServerParams.maxPackageSize = 5000;

    //    NodeCharacteristics anomalySwitchParams;
    //    NodeCharacteristics anomalyBorderGatewayParams;

    // =========================== D_CELL #2 Overloaded Switch =================================
    //    // Requirments
    //    int maxPathLength = 11;
    //    double reqSpeed = 10;
    //    double reqPacketloss = 5;
    //    double reqPing = 2;

    //    // Global sytem params
    //    modelTime_t modelingTimeStep = 400 * g_oneNanosecond;
    //    modelTime_t modelingDuration = 1 * g_oneSecond;
    //    modelTime_t rebuildRoutingTableEvery = 1 * g_oneMillisecond;
    //    bool rebuildRoutingTableGradient = false;
    //    int switchCount = 10;
    //    TopologyType topologyType = TopologyType::D_CELL;
    //    AlgorithmType algorithmType = AlgorithmType::DEIMOS_V1;


    //    // Node params
    //    NodeCharacteristics serverParams;
    //    serverParams.roles = {RoleType::PROVIDER,  RoleType::PRODUCER, RoleType::CONSUMER};
    //    serverParams.bandwidth = 1000;
    //    serverParams.packageProduceFrequency = 100000;
    //    serverParams.minPackageSize = 1000;
    //    serverParams.maxPackageSize = 1500;

    //    NodeCharacteristics switchParams;
    //    switchParams.bandwidth = 10000;

    //    NodeCharacteristics borderGatewayParams;

    //    // Anomaly node params
    //    int partOfAnomalyServers = 0; // %
    //    int partOfAnomalySwitches = 10; // %
    //    int partOfAnomalyBorderGateways = 0; // %

    //    NodeCharacteristics anomalyServerParams;
    //    NodeCharacteristics anomalySwitchParams;
    //    anomalySwitchParams.roles = {RoleType::PROVIDER};
    //    anomalySwitchParams.NetworkInterfaceCount = 1;

    //    NodeCharacteristics anomalyBorderGatewayParams;

// =========================== B_CUBE =================================
//    // Requirments
//    int maxPathLength = 11;
//    double reqSpeed = 100;
//    double reqPacketloss = 3;
//    double reqPing = 1;

//    // Global sytem params
//    modelTime_t modelingTimeStep = 400 * g_oneNanosecond;
//    modelTime_t modelingDuration = 1 * g_oneSecond;
//    modelTime_t rebuildRoutingTableEvery = 1 * g_oneMillisecond;
//    bool rebuildRoutingTableGradient = false;
//    int switchCount = 3;
//    TopologyType topologyType = TopologyType::B_CUBE;
//    AlgorithmType algorithmType = AlgorithmType::DEIMOS_V1;

//    // Node params
//    NodeCharacteristics serverParams;
//    serverParams.roles = {RoleType::PROVIDER, RoleType::CONSUMER};
//    serverParams.bandwidth = 5000;
//    serverParams.bufferVolume = 1;

//    NodeCharacteristics switchParams;
//    switchParams.roles = {RoleType::PROVIDER};
//    switchParams.bufferVolume = 1;
//    switchParams.bandwidth = 1000;

//    NodeCharacteristics borderGatewayParams;
//    borderGatewayParams.roles = {RoleType::PROVIDER, RoleType::PRODUCER};
//    borderGatewayParams.bandwidth = 10000;
//    borderGatewayParams.bufferVolume = 8;
//    borderGatewayParams.packageProduceFrequency = 150000;
//    borderGatewayParams.minPackageSize = 2000;
//    borderGatewayParams.maxPackageSize = 4000;

//    // Anomaly node params
//    int partOfAnomalyServers = 0; // %
//    int partOfAnomalySwitches = 9; // %
//    int partOfAnomalyBorderGateways = 0; // %

//    NodeCharacteristics anomalyServerParams;
//    NodeCharacteristics anomalySwitchParams;
//    anomalySwitchParams.NetworkInterfaceCount = 1;

//    NodeCharacteristics anomalyBorderGatewayParams;

    // Setup model
    auto model = std::make_shared<ModelCore>();
    RUN(model->setupRequirements(maxPathLength, reqSpeed, reqPacketloss, reqPing));
    RUN(model->setupGlobalParams(modelingTimeStep, modelingDuration, rebuildRoutingTableEvery, rebuildRoutingTableGradient, switchCount, topologyType, algorithmType));
    RUN(model->setupNodeParams(serverParams, switchParams, borderGatewayParams));
    RUN(model->setupAnomalyNodeParams(partOfAnomalyServers, partOfAnomalySwitches, partOfAnomalyBorderGateways, anomalyServerParams, anomalySwitchParams, anomalyBorderGatewayParams));
    RUN(model->setup());


    // Setup Qt app
    {
        QApplication a(argc, argv);
        MainWindow w(model);
        w.show();
        status = a.exec();
    }
    
    
exit:
    return status;
}
