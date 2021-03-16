#ifndef MODEL_CORE_HPP
#define MODEL_CORE_HPP

#include <atomic>
#include <future>
#include <memory>

#include "core.hpp"
#include "routingTable.hpp"
#include "node.hpp"

class ModelCore : public CoreBase
{
public:
    status_t start() override;
    status_t stop() override;

    status_t buildNetwork(int nodeCount);

private:
    status_t run();
    status_t buildRoutingTable();

private:
    std::map<hostAddress_t, std::shared_ptr<Node>> m_nodesVec;
    std::shared_ptr<RoutingTable> m_table;
    std::atomic<bool> m_isRunning;
    std::future<status_t> m_runningFut;
};

#endif