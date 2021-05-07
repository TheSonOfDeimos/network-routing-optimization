#include "modelCore.hpp"

#include <iostream>

#include "unitBase.hpp"
#include "time.hpp"
#include "statistic.hpp"
#include "node.hpp"

ModelCore::ModelCore()
    : m_isRunning(false)
{
    m_table = std::make_shared<RoutingTable>();
    m_statistic = std::make_shared<Statistic>();
}

status_t ModelCore::start(modelTime_t duration)
{
    m_thr = std::thread(&ModelCore::run, this);
    return m_thr.joinable() == true ? ERROR_OK : ERROR_FALURE;
}

status_t ModelCore::stop()
{
    if (m_isRunning)
    {
        m_isRunning = false;
        m_thr.join();
        return ERROR_OK;
    }
    else
    {
        return ERROR_LOGIC;
    }
}

status_t ModelCore::setupGlobalParams(modelTime_t modelingTimeStep, modelTime_t modelingDuration, modelTime_t rebuildRoutingTableEvery, bool rebuildRoutingTableGradient, int switchCount, TopologyType topologyType, AlgorithmType algorithmType)
{
    status_t status = ERROR_OK;

    EXIT_IF(modelingTimeStep > modelingDuration, ERROR_LOGIC);

    Time::instance().setStep(modelingTimeStep);
    m_modelingTimeStep = modelingTimeStep;
    m_modelingDuration = modelingDuration;
    m_rebuildRoutingTableEvery = rebuildRoutingTableEvery;
    m_rebuildRoutingTableGradient = rebuildRoutingTableGradient;

    m_switchCount = switchCount;
    EXIT_IF(m_switchCount < 2, ERROR_LOGIC);

    m_topologyType = topologyType;
    m_algorithmType = algorithmType;

exit:
    if (IS_UNSUCCESS(status)) std::cerr << "Invalid time step\n";
    return status;
}

status_t ModelCore::setupNodeParams(NodeCharacteristics serverParams, NodeCharacteristics switchParams, NodeCharacteristics borderGatewayParams)
{
    status_t status = ERROR_OK;

    std::vector<dataVolume_t> minPkgSize;
    std::vector<int> maxPackageFreq;
    dataVolume_t minPackageSize = 0;
    int maxFreq = 0;

    if (serverParams.minPackageSize > 0) minPkgSize.push_back(serverParams.minPackageSize);
    if (switchParams.minPackageSize > 0) minPkgSize.push_back(switchParams.minPackageSize);
    if (borderGatewayParams.minPackageSize > 0) minPkgSize.push_back(borderGatewayParams.minPackageSize);
    if (!minPkgSize.empty())
    {
       minPackageSize = *std::min_element(std::begin(minPkgSize), std::end(minPkgSize));
       double maxBandwidth = std::max({serverParams.bandwidth, borderGatewayParams.bandwidth, switchParams.bandwidth});
       modelTime_t timeToProcess = ((double)minPackageSize * 8) / (maxBandwidth / 1e6);
       EXIT_IF(m_modelingTimeStep * 2 > timeToProcess, ERROR_LOGIC);
    }


    if (serverParams.packageProduceFrequency > 0) maxPackageFreq.push_back(serverParams.packageProduceFrequency);
    if (switchParams.packageProduceFrequency > 0) maxPackageFreq.push_back(switchParams.packageProduceFrequency);
    if (borderGatewayParams.packageProduceFrequency > 0) maxPackageFreq.push_back(borderGatewayParams.packageProduceFrequency);
    if (!minPkgSize.empty())
    {
       maxFreq = *std::max_element(std::begin(maxPackageFreq), std::end(maxPackageFreq));
       EXIT_IF(m_modelingTimeStep > g_oneSecond / maxFreq, ERROR_LOGIC);
    }

    m_serverParams = serverParams;
    m_switchParams = switchParams;
    m_borderGatewayParams = borderGatewayParams;

exit:
    if (IS_UNSUCCESS(status)) std::cerr << "Invalid time step\n";
    return status;
}

status_t ModelCore::setupAnomalyNodeParams(int partOfAnomalyServers, int partOfAnomalySwitches, int partOfAnomalyBorderGateways,
                                           NodeCharacteristics anomalyServerParams, NodeCharacteristics anomalySwitchParams, NodeCharacteristics anomalyBorderGatewayParams)
{
    status_t status = ERROR_OK;

    std::vector<dataVolume_t> minPkgSize;
    std::vector<int> maxPackageFreq;
    dataVolume_t minPackageSize = 0;
    int maxFreq = 0;

    if (anomalyServerParams.minPackageSize > 0) minPkgSize.push_back(anomalyServerParams.minPackageSize);
    if (anomalySwitchParams.minPackageSize > 0) minPkgSize.push_back(anomalySwitchParams.minPackageSize);
    if (anomalyBorderGatewayParams.minPackageSize > 0) minPkgSize.push_back(anomalyBorderGatewayParams.minPackageSize);
    if (!minPkgSize.empty())
    {
       minPackageSize = *std::min_element(std::begin(minPkgSize), std::end(minPkgSize));
       double maxBandwidth = std::max({anomalyServerParams.bandwidth, anomalyBorderGatewayParams.bandwidth, anomalySwitchParams.bandwidth});
       modelTime_t timeToProcess = ((double)minPackageSize * 8) / (maxBandwidth / 1e6);
       EXIT_IF(m_modelingTimeStep * 2 > timeToProcess, ERROR_LOGIC);
    }

    if (anomalyServerParams.packageProduceFrequency > 0) maxPackageFreq.push_back(anomalyServerParams.packageProduceFrequency);
    if (anomalySwitchParams.packageProduceFrequency > 0) maxPackageFreq.push_back(anomalySwitchParams.packageProduceFrequency);
    if (anomalyBorderGatewayParams.packageProduceFrequency > 0) maxPackageFreq.push_back(anomalyBorderGatewayParams.packageProduceFrequency);
    if (!minPkgSize.empty())
    {
       maxFreq = *std::max_element(std::begin(maxPackageFreq), std::end(maxPackageFreq));
       EXIT_IF(m_modelingTimeStep > g_oneSecond / maxFreq, ERROR_LOGIC);
    }

    m_partOfAnomalyServers = partOfAnomalyServers;
    m_partOfAnomalySwitches = partOfAnomalySwitches;
    m_partOfAnomalyBorderGateways = partOfAnomalyBorderGateways;
    m_anomalyServerParams = anomalyServerParams;
    m_anomalySwitchParams = anomalySwitchParams;
    m_anomalyBorderGatewayParams = anomalyBorderGatewayParams;

exit:
    if (IS_UNSUCCESS(status)) std::cerr << "Invalid time step\n";
    return status;
}

status_t ModelCore::setupRequirements(int maxPathLength, double reqSpeed, double reqPacketloss, double reqPing)
{
    status_t status = ERROR_OK;

    m_maxPathLength = maxPathLength;
    m_reqSpeed = reqSpeed;
    m_reqPacketloss = reqPacketloss;
    m_reqPing = reqPing;

    return status;
}

status_t ModelCore::setup()
{
    status_t status = ERROR_OK;

    RUN(buildNetworkTopology());
    RUN(buildRoutingTable());

exit:
    return status;
}

status_t ModelCore::buildNetworkTopology()
{
    status_t status = ERROR_OK;

    std::lock_guard<std::mutex> lock(m_mtx);
    m_nodesVec = TopologyBuilder::build(m_topologyType, m_table, m_switchCount,
                                        m_serverParams, m_switchParams, m_borderGatewayParams,
                                        m_partOfAnomalyServers, m_partOfAnomalySwitches, m_partOfAnomalyBorderGateways,
                                        m_anomalyServerParams, m_anomalySwitchParams, m_anomalyBorderGatewayParams,
                                        m_statistic);
    EXIT_IF(m_nodesVec.size() == 0, ERROR_LOGIC);

exit:
    return status;
}


status_t ModelCore::buildRoutingTable()
{
    status_t status = ERROR_OK;

    std::lock_guard<std::mutex> lock(m_mtx);

    RUN(m_table->buildRoutes(m_algorithmType, m_nodesVec, m_maxPathLength, m_reqSpeed, m_reqPacketloss, m_reqPing));
    EXIT_IF(m_nodesVec.size() == 0, ERROR_LOGIC);

exit:
    return status;
}

std::shared_ptr<Statistic> ModelCore::getStatisticModule()
{
    return m_statistic;
}

status_t ModelCore::run()
{
    status_t status = 0;

    if (m_isRunning)
    {
        return -2;
    }
    else
    {
        m_isRunning = true;
    }

    modelTime_t rebuildRoutingTableGradient = 200 * g_oneMicrosecond;
    while (m_isRunning)
    {
        std::vector<std::future<status_t> > futVec;
        for (auto iter = m_nodesVec.begin(); iter != m_nodesVec.end(); iter++)
        {
            futVec.emplace_back(std::async(std::launch::async, &Node::update, iter->second));
        }
        for (auto iter = futVec.begin(); iter != futVec.end(); iter++)
        {
            RUN(iter->get());
        }

        Time::instance().tick();

        if (m_rebuildRoutingTableEvery != 0 && Time::instance().get() % m_rebuildRoutingTableEvery == 0)
        {
            RUN(buildRoutingTable());
        }

        if (m_rebuildRoutingTableGradient &&
                rebuildRoutingTableGradient < m_rebuildRoutingTableEvery &&
                Time::instance().get() % rebuildRoutingTableGradient == 0)
        {
            RUN(buildRoutingTable());
            rebuildRoutingTableGradient *= 2;
        }



        if (m_modelingDuration != 0 && m_modelingDuration <= Time::instance().get())
        {
            break;
        }
    }

exit:
    m_isRunning = false;
    return status;
}
