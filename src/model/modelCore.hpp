#ifndef MODEL_CORE_HPP
#define MODEL_CORE_HPP

#include <atomic>
#include <future>
#include <memory>

#include "core.hpp"
#include "topologyBuilder.hpp"
#include "routingTable.hpp"

class Statistic;
class Node;

class ModelCore : public CoreBase
{
public:
    ModelCore();

    status_t start(modelTime_t duration = 0) override;
    status_t stop() override;

    status_t setupGlobalParams(modelTime_t modelingTimeStep, modelTime_t modelingDuration, modelTime_t rebuildRoutingTableEvery, bool rebuildRoutingTableGradient, int switchCount, TopologyType topologyType, AlgorithmType algorithmType);
    status_t setupNodeParams(NodeCharacteristics serverParams, NodeCharacteristics switchParams, NodeCharacteristics borderGatewayParams);
    status_t setupAnomalyNodeParams(int partOfAnomalyServers, int partOfAnomalySwitches, int partOfAnomalyBorderGateways,
            NodeCharacteristics anomalyServerParams, NodeCharacteristics anomalySwitchParams, NodeCharacteristics anomalyBorderGatewayParams);
    status_t setupRequirements(int maxPathLength, double reqSpeed, double reqPacketloss, double reqPing);
    status_t setup();

    std::shared_ptr<Statistic> getStatisticModule();

private:
    status_t buildNetworkTopology();
    status_t buildRoutingTable();
    status_t run();

private:
    // Model components
    std::map<hostAddress_t, std::shared_ptr<Node>> m_nodesVec;
    std::shared_ptr<RoutingTable> m_table;
    std::shared_ptr<Statistic> m_statistic;

    // Requirments
    int m_maxPathLength;
    double m_reqSpeed;
    double m_reqPacketloss;
    double m_reqPing;

    // Global sytem params
    modelTime_t m_modelingTimeStep;
    modelTime_t m_modelingDuration;
    modelTime_t m_rebuildRoutingTableEvery;
    bool m_rebuildRoutingTableGradient;
    int m_switchCount;
    TopologyType m_topologyType;
    AlgorithmType m_algorithmType;

    // Node params
    NodeCharacteristics m_serverParams;
    NodeCharacteristics m_switchParams;
    NodeCharacteristics m_borderGatewayParams;

    // Anomaly node params
    int m_partOfAnomalyServers; // %
    int m_partOfAnomalySwitches; // %
    int m_partOfAnomalyBorderGateways; // %
    NodeCharacteristics m_anomalyServerParams;
    NodeCharacteristics m_anomalySwitchParams;
    NodeCharacteristics m_anomalyBorderGatewayParams;

    std::mutex m_mtx;
    std::atomic<bool> m_isRunning;
    std::thread m_thr;
    
};

#endif
