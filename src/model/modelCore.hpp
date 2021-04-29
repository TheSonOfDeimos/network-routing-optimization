#ifndef MODEL_CORE_HPP
#define MODEL_CORE_HPP

#include <atomic>
#include <future>
#include <memory>

#include "core.hpp"
#include "routingTable.hpp"
#include "node.hpp"
#include "topologyBuilder.hpp"

class ModelCore : public CoreBase
{
public:
    ModelCore();

    status_t start(modelTime_t duration = 0) override;
    status_t stop() override;

    status_t buildNetworkTopology(TopologyType type, int switchCount, const NodeCharacteristics& serverParams, const NodeCharacteristics& switchParams);
    
    template <class... Args>
    status_t buildRoutingTable(AlgorithmType type, int maxPathLength, Args &...args);

private:
    status_t run(modelTime_t duration);

private:
    std::map<hostAddress_t, std::shared_ptr<Node>> m_nodesVec;
    std::shared_ptr<RoutingTable> m_table;
    std::mutex m_mtx;
    std::atomic<bool> m_isRunning;
    std::future<status_t> m_runningFut;
    
};

template <class... Args>
status_t ModelCore::buildRoutingTable(AlgorithmType type, int maxPathLength, Args &...args)
{
    status_t status = ERROR_OK;

    std::lock_guard<std::mutex> lock(m_mtx);

    m_table->buildRoutes(type, maxPathLength, m_nodesVec);
    
    EXIT_IF(m_nodesVec.size() == 0, ERROR_LOGIC);

exit:
    return status;
}

#endif
