#include "behaviourSimulator.hpp"

#include <algorithm>

#include "node.hpp"
#include "package.hpp"

BehaviourSimulator::BehaviourSimulator(std::shared_ptr<Node> node)
    : m_role(node->getNodeCharacteristics().roles),
      m_packageProduceFrequency(node->getNodeCharacteristics().packageProduceFrequency),
      m_minPackageSize(node->getNodeCharacteristics().minPackageSize),
      m_maxPackageSize(node->getNodeCharacteristics().maxPackageSize),
      m_node(node)
{
}

status_t BehaviourSimulator::act()
{
    status_t status = ERROR_OK;

    // If one of roles is producer
    if (std::find(m_role.begin(), m_role.end(), RoleType::PRODUCER) != m_role.end())
    {
        if (m_timer.isTimerElapsed())
        {
            auto newPackage = generatePackage();
            RUN(m_node->receive(std::move(newPackage)));
            m_timer.setTimer(g_oneSecond / m_packageProduceFrequency);
        }
    }

    for (auto& processor : m_node->m_processorVec)
    {
        if (processor->isReady())
        {
            auto package = processor->pop();
            
            if (package != nullptr)
            {
                // This node is destination and role consumer
                if (package->destination == m_node->m_params.addr && std::find(m_role.begin(), m_role.end(), RoleType::CONSUMER) != m_role.end())
                {
                    RUN(m_node->send(std::move(package)));
                }
                // This m_node is provider
                else if (package->destination != m_node->m_params.addr && std::find(m_role.begin(), m_role.end(), RoleType::PROVIDER) != m_role.end())
                {
                    RUN(m_node->send(std::move(package)));
                }
            }
            
            RUN(processor->push(m_node->m_queue->pop()));
        }
    }

exit:
    return status;
}

packagePtr_t BehaviourSimulator::generatePackage()
{
    std::srand(std::time(nullptr));
    dataVolume_t volume = m_minPackageSize + (std::rand() % (m_maxPackageSize - m_minPackageSize + 1)); // m_minPackageSize -> m_maxPackageSize
    hostAddress_t source = m_node->m_params.addr;
    hostAddress_t destination = m_node->m_nodeContainer->getRandom(RoleType::CONSUMER)->m_params.addr;

    auto pkg = std::make_unique<Package>(source, destination, volume);
    pkg->inSystem = Time::instance().get();
    return pkg;
}
