#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include <memory>
#include <map>

#include "types.hpp"
#include "package.hpp"
#include "packageQueue.hpp"
#include "unitBase.hpp"
#include "packageProcessor.hpp"
#include "behaviourSimulator.hpp"
#include "nodeContainer.hpp"

class RoutingTable;
class Statistic;

struct NodeCharacteristics
{
    hostAddress_t addr = -1;

    // ==================================== Params ====================================
    // Relative values
    double ping = 0; // ms
    double packetLoss = 0; // amount of lost packets in % per 100 packets
    int packetsTotaly = 0;
    int packetsDropped = 0;
    double speed = 0; // Mbit/sec

    // Absolute values
    int NetworkInterfaceCount = 1; // Number of parallel working network interfaces
    double bandwidth = 100000; // Mbit/sec
    dataVolume_t bufferVolume = 1000; // Mb
    QueuePushRule bufferPushRule = QueuePushRule::BACK;
    QueuePopRule bufferPopRule = QueuePopRule::FRONT;
    QueueDropRule bufferDropRule = QueueDropRule::LAST;

    // Behaviour model
    std::vector<RoleType> roles = {RoleType::PROVIDER};
    int packageProduceFrequency = 0; // Package per second Pkg/sec (if role is PRODUSER)
    dataVolume_t minPackageSize = 0; // Bytes (if role is PRODUSER)
    dataVolume_t maxPackageSize = 0; // Bytes (if role is PRODUSER)

    // ==================================== Requirenments ====================================
    double req_ping = 0.0; // ms
    double req_packetLoss = 0.0; // amount of lost packets in % per 100 packets
    double req_speed = 0.0; // Mbit/sec
    double req_bandwidth = 100000.0; // Mbit/sec

    static hostAddress_t dhcp();
};

class Node : public UnitBase, public std::enable_shared_from_this<Node>
{
public:
    Node(const NodeCharacteristics& ch, std::shared_ptr<RoutingTable> table);
    ~Node();

    // Updates state of this node for current timestamp
    status_t update() override;

    // Sends package to node by calling receive for destination node inside
    status_t send(packagePtr_t pack);
    // Receve package by insetting into network queue (m_queue)
    status_t receive(packagePtr_t pack);
    // Connects nodes to each other
    status_t connectToNode(std::shared_ptr<Node> node);

    std::shared_ptr<NodeContainer> getNodeContainer();
    NodeCharacteristics getNodeCharacteristics();
    std::map<hostAddress_t, std::shared_ptr<Node>> getConnections();

private:
    status_t updateMetrics();

private:
    std::map<hostAddress_t, std::weak_ptr<Node>> m_connections;
    static std::shared_ptr<NodeContainer> m_nodeContainer;

    std::weak_ptr<RoutingTable> m_table;
    std::unique_ptr<PackageQueue> m_queue;
    std::vector<std::unique_ptr<PackageProcessor>> m_processorVec;
    NodeCharacteristics m_params;

    friend class BehaviourSimulator;
    std::unique_ptr<BehaviourSimulator> m_behaviourSimulator;
};

#endif
