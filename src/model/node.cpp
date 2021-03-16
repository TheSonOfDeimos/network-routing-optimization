#include "node.hpp"

hostAddress_t NodeCharacteristics::dhcp()
{
    static std::atomic<hostAddress_t> addr = 1;
    return addr++;
}

Node::Node(const NodeCharacteristics &ch, std::shared_ptr<RoutingTable> table)
    : UnitBase(),
      m_table(table),
      m_queue(ch.bufferVolume, ch.bufferPushRule, ch.bufferPopRule, ch.bufferDropRule),
      m_params(ch)
{
}

status_t Node::update()
{
    return 0;
}

status_t Node::send(packagePtr_t pack)
{
    return 0;
}

status_t Node::connectToNode(std::shared_ptr<Node> node)
{
    status_t status = ERROR_OK;

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