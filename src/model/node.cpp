#include "node.hpp"

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

status_t Node::connectToNode(std::weak_ptr<Node> node)
{
    return 0;
}

NodeCharacteristics Node::getNodeCharacteristics()
{
    return m_params;
}