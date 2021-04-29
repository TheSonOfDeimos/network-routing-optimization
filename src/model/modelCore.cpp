#include "modelCore.hpp"

#include "unitBase.hpp"
#include "time.hpp"

ModelCore::ModelCore()
{
    m_table = std::make_shared<RoutingTable>();
}

status_t ModelCore::start(modelTime_t duration)
{
    m_runningFut = std::async(std::launch::async, &ModelCore::run, this, duration);
    return m_runningFut.valid() == true ? 0 : -1;
}

status_t ModelCore::stop()
{
    if (m_isRunning)
    {
        m_isRunning = false;
        return m_runningFut.get();
    }
    else
    {
        return -2;
    }
}

status_t ModelCore::buildNetworkTopology(TopologyType type, int switchCount, const NodeCharacteristics &serverParams, const NodeCharacteristics &switchParams)
{
    status_t status = ERROR_OK;

    std::lock_guard<std::mutex> lock(m_mtx);

    m_nodesVec = TopologyBuilder::build(type, m_table, switchCount, serverParams, switchParams);

    EXIT_IF(m_nodesVec.size() == 0, ERROR_LOGIC);

exit:
    return status;
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
