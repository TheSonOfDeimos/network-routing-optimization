#include "modelCore.hpp"

#include "unitBase.hpp"
#include "time.hpp"

status_t ModelCore::start()
{
    m_runningFut = std::async(std::launch::async, &ModelCore::run, this);
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

status_t ModelCore::buildNetwork(int nodeCount)
{
    status_t status = 0;
    for(int i = 0; i < nodeCount; i++)
    {
        NodeCharacteristics ch;
        if (m_nodesVec.insert({ch.addr, std::make_shared<Node>(ch, m_table)}).second == false)
        {
            return -1;
        }
    }

    return status;
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

    while (m_isRunning)
    {
        auto iter = beginUnit();
        while (iter != endUnit())
        {
            iter->second->update();
            iter++;
        }
        Time::instance().tick();
    }

    return status;
}