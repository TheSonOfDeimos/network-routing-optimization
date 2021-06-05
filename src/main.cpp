#include <iostream>

#include <QApplication>

#include "model/modelCore.hpp"
#include "gui/mainwindow.hpp"

int main(int argc, char *argv[])
{
    status_t status = ERROR_OK;

// =========================== FAT_TREE #1 Balansed System =================================
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
//    AlgorithmType algorithmType = AlgorithmType::DEIMOS_V1;

//    // Node params
//    NodeCharacteristics serverParams;
//    serverParams.roles = {RoleType::PROVIDER, RoleType::CONSUMER};
//    serverParams.bandwidth = 1000;
//    serverParams.bufferVolume = 1;

//    NodeCharacteristics switchParams;
//    switchParams.roles = {RoleType::PROVIDER};
//    switchParams.bufferVolume = 1;
//    switchParams.bandwidth = 5000;

//    NodeCharacteristics borderGatewayParams;
//    borderGatewayParams.roles = {RoleType::PROVIDER, RoleType::PRODUCER};
//    borderGatewayParams.bandwidth = 10000;
//    borderGatewayParams.bufferVolume = 8;
//    borderGatewayParams.packageProduceFrequency = 150000;
//    borderGatewayParams.minPackageSize = 6000;
//    borderGatewayParams.maxPackageSize = 7000;

//    // Anomaly node params
//    int partOfAnomalyServers = 0; // %
//    int partOfAnomalySwitches = 0; // %
//    int partOfAnomalyBorderGateways = 0; // %

//    NodeCharacteristics anomalyServerParams;
//    NodeCharacteristics anomalySwitchParams;
//    NodeCharacteristics anomalyBorderGatewayParams;

// =========================== FAT_TREE #2 One Overloaded Switch =================================
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
//    int partOfAnomalySwitches = 4; // %
//    int partOfAnomalyBorderGateways = 0; // %

//    NodeCharacteristics anomalyServerParams;
//    NodeCharacteristics anomalySwitchParams;
//    anomalySwitchParams.NetworkInterfaceCount = 1;

//    NodeCharacteristics anomalyBorderGatewayParams;

// =========================== FAT_TREE #3 Overloaded Nodes =================================
//    // Requirments
//    int maxPathLength = 18;
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
//    AlgorithmType algorithmType = AlgorithmType::BELLMAN_FORD;

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
//    int partOfAnomalySwitches = 10; // %
//    int partOfAnomalyBorderGateways = 20; // %

//    NodeCharacteristics anomalyServerParams;
//    NodeCharacteristics anomalySwitchParams;
//    anomalySwitchParams.NetworkInterfaceCount = 2;

//    NodeCharacteristics anomalyBorderGatewayParams;
//    anomalyBorderGatewayParams.roles = {RoleType::PROVIDER, RoleType::PRODUCER};
//    anomalyBorderGatewayParams.bandwidth = 10000;
//    anomalyBorderGatewayParams.bufferVolume = 8;
//    anomalyBorderGatewayParams.packageProduceFrequency = 170000;
//    anomalyBorderGatewayParams.minPackageSize = 4500;
//    anomalyBorderGatewayParams.maxPackageSize = 5000;


    // =========================== D_CELL #1 Balansed system =================================
//        // Requirments
//        int maxPathLength = 11;
//        double reqSpeed = 10;
//        double reqPacketloss = 30;
//        double reqPing = 2;

//        // Global sytem params
//        modelTime_t modelingTimeStep = 400 * g_oneNanosecond;
//        modelTime_t modelingDuration = 1 * g_oneSecond;
//        modelTime_t rebuildRoutingTableEvery = 1 * g_oneMillisecond;
//        bool rebuildRoutingTableGradient = false;
//        int switchCount = 15;
//        TopologyType topologyType = TopologyType::D_CELL;
//        AlgorithmType algorithmType = AlgorithmType::DIJKSTRA;


//        // Node params
//        NodeCharacteristics serverParams;
//        serverParams.roles = {RoleType::PROVIDER,  RoleType::PRODUCER, RoleType::CONSUMER};
//        serverParams.bandwidth = 1000;
//        serverParams.packageProduceFrequency = 100000;
//        serverParams.minPackageSize = 1000;
//        serverParams.maxPackageSize = 1500;

//        NodeCharacteristics switchParams;
//        switchParams.roles = {RoleType::PROVIDER};
//        switchParams.bandwidth = 10000;

//        NodeCharacteristics borderGatewayParams;

//        // Anomaly node params
//        int partOfAnomalyServers = 0; // %
//        int partOfAnomalySwitches = 0; // %
//        int partOfAnomalyBorderGateways = 0; // %

//        NodeCharacteristics anomalyServerParams;
//        NodeCharacteristics anomalySwitchParams;
//        NodeCharacteristics anomalyBorderGatewayParams;

    // =========================== D_CELL #2 One Oveloaded Server =================================
//        // Requirments
//        int maxPathLength = 11;
//        double reqSpeed = 10;
//        double reqPacketloss = 30;
//        double reqPing = 2;

//        // Global sytem params
//        modelTime_t modelingTimeStep = 400 * g_oneNanosecond;
//        modelTime_t modelingDuration = 1 * g_oneSecond;
//        modelTime_t rebuildRoutingTableEvery = 1 * g_oneMillisecond;
//        bool rebuildRoutingTableGradient = false;
//        int switchCount = 15;
//        TopologyType topologyType = TopologyType::D_CELL;
//        AlgorithmType algorithmType = AlgorithmType::BELLMAN_FORD;


//        // Node params
//        NodeCharacteristics serverParams;
//        serverParams.roles = {RoleType::PROVIDER,  RoleType::PRODUCER, RoleType::CONSUMER};
//        serverParams.bandwidth = 1000;
//        serverParams.packageProduceFrequency = 100000;
//        serverParams.minPackageSize = 1000;
//        serverParams.maxPackageSize = 1500;

//        NodeCharacteristics switchParams;
//        switchParams.bandwidth = 10000;

//        NodeCharacteristics borderGatewayParams;

//        // Anomaly node params
//        int partOfAnomalyServers = 1; // %
//        int partOfAnomalySwitches = 0; // %
//        int partOfAnomalyBorderGateways = 0; // %

//        NodeCharacteristics anomalyServerParams;
//        anomalyServerParams.roles = {RoleType::PROVIDER, RoleType::PRODUCER, RoleType::CONSUMER};
//        anomalyServerParams.bandwidth = 1000;
//        anomalyServerParams.packageProduceFrequency = 80000;
//        anomalyServerParams.minPackageSize = 4000;
//        anomalyServerParams.maxPackageSize = 5000;

//        NodeCharacteristics anomalySwitchParams;
//        NodeCharacteristics anomalyBorderGatewayParams;

        // =========================== D_CELL #3 Oveloaded Nodes =================================
//            // Requirments
//            int maxPathLength = 11;
//            double reqSpeed = 10;
//            double reqPacketloss = 30;
//            double reqPing = 2;

//            // Global sytem params
//            modelTime_t modelingTimeStep = 400 * g_oneNanosecond;
//            modelTime_t modelingDuration = 1 * g_oneSecond;
//            modelTime_t rebuildRoutingTableEvery = 1 * g_oneMillisecond;
//            bool rebuildRoutingTableGradient = false;
//            int switchCount = 15;
//            TopologyType topologyType = TopologyType::D_CELL;
//            AlgorithmType algorithmType = AlgorithmType::BELLMAN_FORD;


//            // Node params
//            NodeCharacteristics serverParams;
//            serverParams.roles = {RoleType::PROVIDER,  RoleType::PRODUCER, RoleType::CONSUMER};
//            serverParams.bandwidth = 1000;
//            serverParams.packageProduceFrequency = 100000;
//            serverParams.minPackageSize = 1000;
//            serverParams.maxPackageSize = 1500;

//            NodeCharacteristics switchParams;
//            switchParams.bandwidth = 10000;

//            NodeCharacteristics borderGatewayParams;

//            // Anomaly node params
//            int partOfAnomalyServers = 7; // %
//            int partOfAnomalySwitches = 8; // %
//            int partOfAnomalyBorderGateways = 0; // %

//            NodeCharacteristics anomalyServerParams;
//            anomalyServerParams.roles = {RoleType::PROVIDER, RoleType::PRODUCER, RoleType::CONSUMER};
//            anomalyServerParams.bandwidth = 1000;
//            anomalyServerParams.packageProduceFrequency = 50000;
//            anomalyServerParams.minPackageSize = 4000;
//            anomalyServerParams.maxPackageSize = 5000;

//            NodeCharacteristics anomalySwitchParams;
//            anomalySwitchParams.roles = {RoleType::PROVIDER};
//            anomalySwitchParams.NetworkInterfaceCount = 2;

//            NodeCharacteristics anomalyBorderGatewayParams;


// =========================== B_CUBE #1 Balansed System =================================
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
//    int switchCount = 8;
//    TopologyType topologyType = TopologyType::B_CUBE;
//    AlgorithmType algorithmType = AlgorithmType::DEIMOS_V1;

//    // Node params
//    NodeCharacteristics serverParams;
//    serverParams.roles = {RoleType::PROVIDER,  RoleType::PRODUCER, RoleType::CONSUMER};
//    serverParams.bandwidth = 1000;
//    serverParams.packageProduceFrequency = 100000;
//    serverParams.minPackageSize = 1000;
//    serverParams.maxPackageSize = 1500;

//    NodeCharacteristics switchParams;
//    switchParams.roles = {RoleType::PROVIDER};
//    switchParams.bandwidth = 5000;

//    NodeCharacteristics borderGatewayParams;
//    borderGatewayParams.roles = {RoleType::PROVIDER, RoleType::PRODUCER};
//    borderGatewayParams.bandwidth = 10000;
//    borderGatewayParams.bufferVolume = 8;
//    borderGatewayParams.packageProduceFrequency = 150000;
//    borderGatewayParams.minPackageSize = 4000;
//    borderGatewayParams.maxPackageSize = 5000;

//    // Anomaly node params
//    int partOfAnomalyServers = 0; // %
//    int partOfAnomalySwitches = 0; // %
//    int partOfAnomalyBorderGateways = 0; // %

//    NodeCharacteristics anomalyServerParams;
//    NodeCharacteristics anomalySwitchParams;
//    NodeCharacteristics anomalyBorderGatewayParams;

// =========================== B_CUBE #2 One Oveloaded Server =================================
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
//    int switchCount = 8;
//    TopologyType topologyType = TopologyType::B_CUBE;
//    AlgorithmType algorithmType = AlgorithmType::BELLMAN_FORD;

//    // Node params
//    NodeCharacteristics serverParams;
//    serverParams.roles = {RoleType::PROVIDER,  RoleType::PRODUCER, RoleType::CONSUMER};
//    serverParams.bandwidth = 1000;
//    serverParams.packageProduceFrequency = 100000;
//    serverParams.minPackageSize = 1000;
//    serverParams.maxPackageSize = 1500;

//    NodeCharacteristics switchParams;
//    switchParams.roles = {RoleType::PROVIDER};
//    switchParams.bandwidth = 5000;

//    NodeCharacteristics borderGatewayParams;
//    borderGatewayParams.roles = {RoleType::PROVIDER, RoleType::PRODUCER};
//    borderGatewayParams.bandwidth = 10000;
//    borderGatewayParams.bufferVolume = 8;
//    borderGatewayParams.packageProduceFrequency = 150000;
//    borderGatewayParams.minPackageSize = 4000;
//    borderGatewayParams.maxPackageSize = 5000;

//    // Anomaly node params
//    int partOfAnomalyServers = 2; // %
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

// =========================== B_CUBE #3 One Oveloaded Nodes =================================
//    // Requirments
//    int maxPathLength = 18;
//    double reqSpeed = 100;
//    double reqPacketloss = 7;
//    double reqPing = 0.8;

//    // Global sytem params
//    modelTime_t modelingTimeStep = 400 * g_oneNanosecond;
//    modelTime_t modelingDuration = 1 * g_oneSecond;
//    modelTime_t rebuildRoutingTableEvery = 1 * g_oneMillisecond;
//    bool rebuildRoutingTableGradient = false;
//    int switchCount = 8;
//    TopologyType topologyType = TopologyType::B_CUBE;
//    AlgorithmType algorithmType = AlgorithmType::BELLMAN_FORD;

//    // Node params
//    NodeCharacteristics serverParams;
//    serverParams.roles = {RoleType::PROVIDER,  RoleType::PRODUCER, RoleType::CONSUMER};
//    serverParams.bandwidth = 1000;
//    serverParams.packageProduceFrequency = 100000;
//    serverParams.minPackageSize = 1000;
//    serverParams.maxPackageSize = 1500;

//    NodeCharacteristics switchParams;
//    switchParams.roles = {RoleType::PROVIDER};
//    switchParams.bandwidth = 5000;

//    NodeCharacteristics borderGatewayParams;
//    borderGatewayParams.roles = {RoleType::PROVIDER, RoleType::PRODUCER};
//    borderGatewayParams.bandwidth = 10000;
//    borderGatewayParams.bufferVolume = 8;
//    borderGatewayParams.packageProduceFrequency = 150000;
//    borderGatewayParams.minPackageSize = 4000;
//    borderGatewayParams.maxPackageSize = 5000;

//    // Anomaly node params
//    int partOfAnomalyServers = 10; // %
//    int partOfAnomalySwitches = 15; // %
//    int partOfAnomalyBorderGateways = 15; // %

//    NodeCharacteristics anomalyServerParams;
//    anomalyServerParams.roles = {RoleType::PROVIDER, RoleType::PRODUCER, RoleType::CONSUMER};
//    anomalyServerParams.bandwidth = 1000;
//    anomalyServerParams.packageProduceFrequency = 80000;
//    anomalyServerParams.minPackageSize = 2000;
//    anomalyServerParams.maxPackageSize = 3000;

//    NodeCharacteristics anomalySwitchParams;
//    anomalySwitchParams.roles = {RoleType::PROVIDER};
//    anomalySwitchParams.bandwidth = 5000;
//    anomalySwitchParams.NetworkInterfaceCount = 2;

//    NodeCharacteristics anomalyBorderGatewayParams;
//    anomalyBorderGatewayParams.roles = {RoleType::PROVIDER, RoleType::PRODUCER};
//    anomalyBorderGatewayParams.bandwidth = 10000;
//    anomalyBorderGatewayParams.bufferVolume = 8;
//    anomalyBorderGatewayParams.packageProduceFrequency = 180000;
//    anomalyBorderGatewayParams.minPackageSize = 5000;
//    anomalyBorderGatewayParams.maxPackageSize = 6000;

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
