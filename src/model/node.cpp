#include "node.hpp"

#include "routingTable.hpp"
#include "statistic.hpp"

hostAddress_t NodeCharacteristics::dhcp()
{
    static std::atomic<hostAddress_t> addr = 1;
    return addr++;
}

std::shared_ptr<NodeContainer>  Node::m_nodeContainer;

Node::Node(const NodeCharacteristics &ch, std::shared_ptr<RoutingTable> table)
    : UnitBase(),
      m_table(table),
      m_params(ch)
{
    m_params.addr = NodeCharacteristics::dhcp();
    m_queue = std::make_unique<PackageQueue>(0, ch.bufferVolume, ch.bufferPushRule, ch.bufferPopRule, ch.bufferDropRule);

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

    if (m_behaviourSimulator == nullptr)
    {
        m_behaviourSimulator = std::make_unique<BehaviourSimulator>(shared_from_this());
    }
    
    RUN(m_behaviourSimulator->act());
    Statistic::instance().report(m_params);

exit:
    return status;
}

status_t Node::send(packagePtr_t pack)
{
    status_t status = ERROR_OK;

    EXIT_IF(pack == nullptr, ERROR_NO_EFFECT);

    // This node is destination
    if (pack->destination == m_params.addr)
    {
        pack->outSystem = Time::instance().get();
        pack->isReached = true;
        pack->lastNode = m_params.addr;
        Statistic::instance().report(std::move(pack));
        EXIT(ERROR_OK);
    }

    {
        Route route = m_table.lock()->get(pack->source, pack->destination);
        auto self = std::find(route.path.begin(), route.path.end(), m_params.addr);

        EXIT_IF(self == route.path.end(), ERROR_LOGIC);

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

    m_params.packetsTotaly = m_queue->getTotal();
    m_params.packetsDropped = m_queue->getDrops();
    m_params.packetLoss = m_params.packetsTotaly / m_params.packetsDropped * 100;

    return status;
}
