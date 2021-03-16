#ifndef ALGORITHM_BASE_HPP
#define ALGORITHM_BASE_HPP

#include <unordered_map>
#include <vector>

#include "types.hpp"

struct Cell
{
    // Relative values
    double ping; // ms
    double packetLoss; // amount of lost packets in % per 100 packets
    double packetDrop; // amount of dropped packets from queue in % per 100 packets
    double speed; // Mbit/sec

    // Absolute values
    double bandwidth; // Mbit/sec
    dataVolume_t bufferVolume; // Mb

    double cost = -1;
    std::vector<hostAddress_t> path = {};
};

// On Input
using ConnectMatrix_t = std::unordered_map<hostAddress_t, std::unordered_map<hostAddress_t, Cell>>;

struct Route
{
    double cost;
    std::vector<hostAddress_t> path;
    hostAddress_t source;
    hostAddress_t destination;
};

// On Output
using RouteTable_t = std::vector<Route>;

class AlgorithmBase
{
public:
    virtual ~AlgorithmBase() = default;
    RouteTable_t buildRoutes(const ConnectMatrix_t& matrix);

protected:
    ConnectMatrix_t buildNext(const ConnectMatrix_t& startMatrix, const ConnectMatrix_t& prevMatrix);
    
    virtual status_t adoptStartMatrix(ConnectMatrix_t& startMatrix) = 0;
    virtual status_t isPathPossible(hostAddress_t startLineAddr, const std::pair<hostAddress_t, Cell>& startElement, hostAddress_t prevLineAddr, const std::pair<hostAddress_t, Cell>& prevElement) = 0;
    virtual status_t appentToNextMatrix(hostAddress_t startLineAddr, const std::pair<hostAddress_t, Cell>& startElement, hostAddress_t prevLineAddr, const std::pair<hostAddress_t, Cell>& prevElement, ConnectMatrix_t& nextMatrix) = 0;
    virtual RouteTable_t mergeMatrixToRouteTable(const std::vector<ConnectMatrix_t>& matrixVec) = 0;
};

#endif