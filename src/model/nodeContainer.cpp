#include "nodeContainer.hpp"

#include "node.hpp"

status_t NodeContainer::add(Node* node, hostAddress_t addr, const std::vector<RoleType>& roles)
{
    status_t status = ERROR_OK;

    std::lock_guard<std::mutex> lock(m_mtx);

    EXIT_IF(m_nodesByAddress.insert({addr, node}).second == false, ERROR_LOGIC);

    for (auto role : roles)
    {
        m_nodesByRole.insert({role, node});
    }

exit:
    return status;
}

status_t NodeContainer::remove(Node * node)
{
    status_t status = ERROR_OK;

    std::lock_guard<std::mutex> lock(m_mtx);

    EXIT_IF(m_nodesByAddress.erase(node->getNodeCharacteristics().addr) == 0, ERROR_NO_EFFECT);

    for (auto role : node->getNodeCharacteristics().roles)
    {
        auto range = m_nodesByRole.equal_range(role);
        auto iterToRemove = std::find_if(range.first, range.second, [node](auto& el){return el.second == node;});

        EXIT_IF(iterToRemove == m_nodesByRole.end(), ERROR_NO_EFFECT);

        m_nodesByRole.erase(iterToRemove);
    }

exit:
    return status;
}

std::shared_ptr<Node> NodeContainer::get(hostAddress_t addr)
{
    std::lock_guard<std::mutex> lock(m_mtx);

    auto foundIter = m_nodesByAddress.find(addr);
    if (foundIter == m_nodesByAddress.end())
    {
        return nullptr;
    }

    return foundIter->second->shared_from_this();
}

std::vector<std::shared_ptr<Node>> NodeContainer::get(RoleType role)
{
    std::lock_guard<std::mutex> lock(m_mtx);

    std::vector<std::shared_ptr<Node>> res = {};

    auto range = m_nodesByRole.equal_range(role);
    
    for (auto it = range.first; it != range.second; it++)
    {
        res.emplace_back(it->second->shared_from_this());
    }

    return res;
}

std::shared_ptr<Node> NodeContainer::getRandom()
{
    std::lock_guard<std::mutex> lock(m_mtx);

    std::srand(std::time(nullptr));
    int randomIndex = std::rand() % m_nodesByAddress.size();
    auto iter = m_nodesByAddress.begin();
    for (int i = 0; i < randomIndex; i++)
    {
        iter++;
    }

    return iter->second->shared_from_this();
}

std::shared_ptr<Node> NodeContainer::getRandom(RoleType role)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    
    auto range = m_nodesByRole.equal_range(role);
    int size = std::distance(range.first, range.second);

    std::srand(std::time(nullptr));
    int randomIndex = std::rand() % size; // 0 - size
    auto iter = range.first;
    for (int i = 0; i < randomIndex; i++)
    {
        iter++;
    }

    return iter->second->shared_from_this();
}
