#include "node.hpp"

#include "routingTable.hpp"

hostAddress_t NodeCharacteristics::dhcp()
{
    static std::atomic<hostAddress_t> addr = 1;
    return addr++;
}

Node::Node(const NodeCharacteristics &ch, std::shared_ptr<RoutingTable> table)
    : UnitBase(),
      m_table(table),
      m_queue(ch.bufferVolume, ch.bufferPushRule, ch.bufferPopRule, ch.bufferDropRule),
      m_processor(ch.bandwidth),
      m_params(ch)
{
}

status_t Node::update()
{
    status_t status = ERROR_OK;

    if (m_processor.isReady())
    {
        RUN(send(std::move(m_processor.pop())));
        RUN(m_processor.push(std::move(m_queue.pop())));
    }

exit:
    return status;
}

status_t Node::send(packagePtr_t pack)
{
    status_t status = ERROR_OK;

    EXIT_IF(pack == nullptr, ERROR_NO_EFFECT);

    // This node is destination
    EXIT_IF(pack->destination == m_params.addr, ERROR_OK);

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
    RUN(m_queue.push(std::move(pack)));
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

std::map<hostAddress_t, std::shared_ptr<Node>> Node::getConnections()
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