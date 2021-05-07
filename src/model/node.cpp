#include "node.hpp"

#include <iostream>

#include "routingTable.hpp"
#include "statistic.hpp"

bool operator == (const std::vector<RoleType>& l, const std::vector<RoleType>& r)
{
    try
    {
        if (l.size() != r.size()) return false;
        if (l.empty() && r.empty()) return true;
        if (l.size() == 3 && r.size() == 3) return true;
        if (l.size() == 1 && r.size() == 1 && l.front() == r.front()) return true;
        if ((l.size() == 2 && r.size() == 2) && ((l.at(0) == r.at(0) && l.at(1) == r.at(1)) || (l.at(1) == r.at(0) && l.at(0) == r.at(1)))) return true;
    }
    catch (std::exception& e)
    {
        std::cerr << e.what();
    }
    return false;
}

hostAddress_t NodeCharacteristics::dhcp()
{
    static std::atomic<hostAddress_t> addr = 1;
    return addr++;
}

std::shared_ptr<NodeContainer>  Node::m_nodeContainer;

Node::Node(const NodeCharacteristics &ch, std::shared_ptr<RoutingTable> table, std::shared_ptr<Statistic> stat)
    : UnitBase(),
      m_table(table),
      m_params(ch),
      m_statistic(stat)
{
    std::srand(std::time(nullptr));
    m_params.addr = NodeCharacteristics::dhcp();
    m_queue = std::make_unique<PackageQueue>(m_statistic, m_params.addr, ch.bufferVolume, ch.bufferPushRule, ch.bufferPopRule, ch.bufferDropRule);

    if (m_nodeContainer == nullptr)
    {
        m_nodeContainer = std::make_shared<NodeContainer>();
    }

    m_nodeContainer->add(this, m_params.addr, m_params.roles);

    for (int i = 0; i < m_params.NetworkInterfaceCount; i++)
    {
        m_processorVec.emplace_back(std::make_unique<PackageProcessor>(m_params.bandwidth));
    }
}

Node::~Node()
{
    m_nodeContainer->remove(this);
}

std::shared_ptr<NodeContainer> Node::getNodeContainer()
{
    return m_nodeContainer;
}

status_t Node::update()
{
    status_t status = ERROR_OK;

    // If one of roles is producer
    if (std::find(m_params.roles.begin(), m_params.roles.end(), RoleType::PRODUCER) != m_params.roles.end())
    {
        if (m_timer.isTimerElapsed())
        {
            auto newPackage = generatePackage();
            EXIT_IF(newPackage == nullptr, ERROR_FALURE);
            RUN(receive(std::move(newPackage)));
            m_timer.setTimer(g_oneSecond / (modelTime_t)m_params.packageProduceFrequency);
        }
    }

    for (auto& processor : m_processorVec)
    {
        if (processor->isReady())
        {
            auto package = processor->pop();

            if (package != nullptr)
            {
                // This node is destination and role consumer
                if (package->destination == m_params.addr && std::find(m_params.roles.begin(), m_params.roles.end(), RoleType::CONSUMER) != m_params.roles.end())
                {
                    package->outSystem = Time::instance().get();
                    package->isReached = true;
                    package->lastNode = m_params.addr;
                    RUN(m_statistic->report(std::move(package)));
                }
                // This m_node is provider
                else if (package->destination != m_params.addr && std::find(m_params.roles.begin(), m_params.roles.end(), RoleType::PROVIDER) != m_params.roles.end())
                {
                    RUN(send(std::move(package)));
                }
                else
                {
                    EXIT(ERROR_LOGIC);
                }
            }

            RUN(processor->push(m_queue->pop()));
        }
    }
    
    RUN(updateMetrics());
    RUN(m_statistic->report(m_params));

exit:
    return status;
}

status_t Node::send(packagePtr_t pack)
{
    status_t status = ERROR_OK;

    EXIT_IF(pack == nullptr, ERROR_NO_EFFECT);
    {
        auto self = std::find(pack->path.begin(), pack->path.end(), m_params.addr);

        EXIT_IF(self == pack->path.end(), ERROR_LOGIC);

        // Trying to get next dest node
        self++;
        auto destNode = m_connections.find(*self);

        EXIT_IF(destNode == m_connections.end(), ERROR_LOGIC);
        RUN(destNode->second.lock()->receive(std::move(pack)));
    }
exit:
    return status;
}

status_t Node::receive(packagePtr_t pack)
{
    status_t status = ERROR_OK;
    EXIT_IF(pack == nullptr, ERROR_NO_EFFECT);
    RUN(m_queue->push(std::move(pack)));
exit:
    return status;
}

status_t Node::connectToNode(std::shared_ptr<Node> node)
{
    status_t status = ERROR_OK;

    EXIT_IF(node == nullptr, ERROR_NO_EFFECT);

    //#ifdef DEBUG
    if ((this->m_connections.count(node->getNodeCharacteristics().addr) == 0) && (node->m_connections.count(this->getNodeCharacteristics().addr) > 0))
    {
        EXIT(ERROR_LOGIC);
    }
    if ((this->m_connections.count(node->getNodeCharacteristics().addr) > 0) && (node->m_connections.count(this->getNodeCharacteristics().addr) == 0))
    {
        EXIT(ERROR_LOGIC);
    }
    //#endif

    {
        bool res_1 = this->m_connections.insert({node->m_params.addr, node}).second;
        bool res_2 = node->m_connections.insert({this->m_params.addr, shared_from_this()}).second;
        if (res_1 == false && res_2 == false)
        {
            EXIT(ERROR_NO_EFFECT);
        }
    }

    //#ifdef DEBUG
    if ((this->m_connections.count(node->getNodeCharacteristics().addr) == 0) && (node->m_connections.count(this->getNodeCharacteristics().addr) > 0))
    {
        EXIT(ERROR_LOGIC);
    }
    if ((this->m_connections.count(node->getNodeCharacteristics().addr) > 0) && (node->m_connections.count(this->getNodeCharacteristics().addr) == 0))
    {
        EXIT(ERROR_LOGIC);
    }
    //#endif

exit:
    return status;
}

NodeCharacteristics Node::getNodeCharacteristics()
{
    return m_params;
}

std::map<hostAddress_t, std::shared_ptr<Node> > Node::getConnections()
{
    std::map<hostAddress_t, std::shared_ptr<Node> > connections;
    auto iter = m_connections.begin();
    while (iter != m_connections.end())
    {
        // Clean if node is no longer exists
        if (iter->second.expired())
        {
            iter = m_connections.erase(iter);
            continue;
        }
        connections.insert({iter->first, iter->second.lock()});
        iter++;
    }
    return connections;
}

status_t Node::updateMetrics()
{
    status_t status = ERROR_OK;


    m_params.ping = m_queue->getPing();
    m_params.packetLoss = m_queue->getPacketLoss();

    m_params.speed = 0;
    for (auto& proc : m_processorVec)
    {
       m_params.speed += proc->getSpeed();
    }
    m_params.speed /= m_processorVec.size();

    return status;
}

packagePtr_t Node::generatePackage()
{
    status_t status = ERROR_OK;
    dataVolume_t volume = m_params.minPackageSize + (std::rand() % (m_params.maxPackageSize - m_params.minPackageSize + 1)); // m_minPackageSize -> m_maxPackageSize
    hostAddress_t source = m_params.addr;
    hostAddress_t destination = m_nodeContainer->getRandom(RoleType::CONSUMER, m_params.addr)->m_params.addr;

    auto pkg = std::make_unique<Package>(source, destination, volume);
    pkg->inSystem = Time::instance().get();

    // Tying to get route
    Route route = m_table.lock()->get(source, destination);
    auto self = std::find(route.path.begin(), route.path.end(), m_params.addr);
    EXIT_IF(self == route.path.end(), ERROR_LOGIC);

    pkg->path = route.path;

exit:
    if (IS_UNSUCCESS(status)) return nullptr;
    return pkg;
}
