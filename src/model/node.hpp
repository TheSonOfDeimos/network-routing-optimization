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

class RoutingTable;

class NodeCharacteristics
{
public:
    hostAddress_t addr = dhcp();

    // Relative values
    double ping = 0; // ms
    double packetLoss = 0; // amount of lost packets in % per 100 packets
    double packetDrop = 0; // amount of dropped packets from queue in % per 100 packets
    double speed = 0; // Mbit/sec

    // Absolute values
    int NetworkInterfaceCount = 1; // Number of parallel working network interfaces
    double bandwidth = 100000; // Mbit/sec
    dataVolume_t bufferVolume = 1000; // Mb
    QueuePushRule bufferPushRule;
    QueuePopRule bufferPopRule;
    QueueDropRule bufferDropRule;

private:
    static hostAddress_t dhcp();
};

class Node : public UnitBase, public std::enable_shared_from_this<Node>
{
public:
    Node(const NodeCharacteristics& ch, std::shared_ptr<RoutingTable> table);

    // Updates state of this node for current timestamp
    status_t update() override;

    // Sends package to node by calling receive for destination node inside
    status_t send(packagePtr_t pack);
    // Receve package by insetting into network queue (m_queue)
    status_t receive(packagePtr_t pack);
    // Connects nodes to each other
    status_t connectToNode(std::shared_ptr<Node> node);

    NodeCharacteristics getNodeCharacteristics();
    std::map<hostAddress_t, std::shared_ptr<Node>> getConnections();

private:
    std::map<hostAddress_t, std::weak_ptr<Node>> m_connections;
    std::weak_ptr<RoutingTable> m_table;
    PackageQueue m_queue;
    PackageProcessor m_processor;
    NodeCharacteristics m_params;
};

#endif