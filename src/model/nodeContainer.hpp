#ifndef NODE_CONTAINER_HPP
#define NODE_CONTAINER_HPP

#include <unordered_map>
#include <mutex>

#include "types.hpp"
#include "behaviourSimulator.hpp"

class Node;

class NodeContainer
{
public:
    status_t add(Node* node, hostAddress_t addr, const std::vector<RoleType>& roles);
    status_t remove(Node* node);

    std::shared_ptr<Node> get(hostAddress_t addr);
    std::vector<std::shared_ptr<Node>> get(RoleType role);
    std::shared_ptr<Node> getRandom();
    std::shared_ptr<Node> getRandom(RoleType role);

private:
    std::unordered_multimap<RoleType, Node*> m_nodesByRole;
    std::unordered_map<hostAddress_t, Node*> m_nodesByAddress;

    std::mutex m_mtx;
};

#endif
