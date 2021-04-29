#ifndef BEHAVIOUR_SIMULATOR_HPP
#define BEHAVIOUR_SIMULATOR_HPP

#include <memory>
#include <vector>

#include "types.hpp"
#include "time.hpp"
#include "package.hpp"

class Node;

enum class RoleType : int
{
    CONSUMER = 0,
    PRODUCER,
    PROVIDER
};

class BehaviourSimulator
{
public:
    BehaviourSimulator(std::shared_ptr<Node> node);

    status_t act();

private:
    packagePtr_t generatePackage();

private:
    std::vector<RoleType> m_role;
    int m_packageProduceFrequency; // Package per second Pkg/sec (if role is PRODUSER)
    dataVolume_t m_minPackageSize; // Bytes (if role is PRODUSER)
    dataVolume_t m_maxPackageSize; // Bytes (if role is PRODUSER)

    std::shared_ptr<Node> m_node;

    Timer m_timer;
};

#endif