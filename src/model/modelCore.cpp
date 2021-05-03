#include "modelCore.hpp"

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
    m_thr = std::thread(&ModelCore::run, this, duration);
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

status_t ModelCore::buildNetworkTopology(TopologyType type, int switchCount, const NodeCharacteristics &serverParams, const NodeCharacteristics &switchParams)
{
    status_t status = ERROR_OK;

    std::lock_guard<std::mutex> lock(m_mtx);

    m_nodesVec = TopologyBuilder::build(type, m_table, switchCount, serverParams, switchParams, m_statistic);

    EXIT_IF(m_nodesVec.size() == 0, ERROR_LOGIC);

exit:
    return status;
}

std::shared_ptr<Statistic> ModelCore::getStatisticModule()
{
    return m_statistic;
}

status_t ModelCore::run(modelTime_t duration)
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

        if (duration != 0 && duration <= Time::instance().get())
        {
            break;
        }
    }

exit:
    m_isRunning = false;
    return status;
}
