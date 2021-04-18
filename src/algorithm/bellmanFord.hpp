#ifndef BELLMAN_FORD_HPP
#define BELLMAN_FORD_HPP

#include "algorithmBase.hpp"

class BellmanFord : public AlgorithmBase
{
private:
    status_t adoptStartMatrix(ConnectMatrix_t& startMatrix) override;
    status_t isPathPossible(hostAddress_t startLineAddr, const std::pair<hostAddress_t, Cell>& startElement, hostAddress_t prevLineAddr, const std::pair<hostAddress_t, Cell>& prevElement) override;
    status_t appentToNextMatrix(hostAddress_t startLineAddr, const std::pair<hostAddress_t, Cell>& startElement, hostAddress_t prevLineAddr, const std::pair<hostAddress_t, Cell>& prevElement, ConnectMatrix_t& nextMatrix) override;
    RouteTable_t mergeMatrixToRouteTable(const std::vector<ConnectMatrix_t>& matrixVec) override;
};

#endif