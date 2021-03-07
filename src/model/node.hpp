#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include <memory>

#include "types.hpp"
#include "package.hpp"
#include "packageQueue.hpp"
#include "routingTable.hpp"
#include "unitBase.hpp"

class NodeCharacteristics
{
public:
    hostAddress_t addr = dhcp();
    hostName_t name = "NONE";
    hostId_t id = counter();

    // Relative values
    double ping = 0; // ms
    double packetLoss = 0; // amount of lost packets in % per 100 packets
    double packetDrop = 0; // amount of dropped packets from queue in % per 100 packets
    double speed = 0; // Mbit/sec

    // Absolute values
    int NetworkInterfaceCount = 1; // Number of parallel working network interfaces
    double bufferSize = 100; // Mb
    double bandwidth = 1000; // Mbit/sec

private:
    static hostId_t counter() 
    {
        static std::atomic<hostId_t> idCounter;
        return idCounter++;
    }

    static hostAddress_t dhcp() 
    {
        static std::atomic<hostId_t> fourthOct = 0;
        return "192.168.1." + std::to_string(fourthOct++);
    }
};

class Node : public UnitBase
{
public:
    Node(const NodeCharacteristics& ch, std::shared_ptr<RoutingTable> table);

    status_t update() override;

    status_t send(packagePtr_t pack);
    status_t connectToNode(std::weak_ptr<Node> node);

    NodeCharacteristics getNodeCharacteristics();

private:
    hostAddress_t m_address;
    std::vector<std::weak_ptr<Node>> m_connections;
    std::shared_ptr<RoutingTable> m_table;
    PackageQueue m_queue;
    NodeCharacteristics m_params;
};

#endif