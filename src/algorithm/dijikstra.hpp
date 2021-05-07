#ifndef DIJIKSTRA_HPP
#define DIJIKSTRA_HPP

#include "algorithmBase.hpp"

class Dijikstra : public AlgorithmBase
{
public:
    Dijikstra(int maxPathLength, double, double, double);

private:
    status_t adoptStartMatrix(ConnectMatrix_t& startMatrix) override;
    status_t isPathPossible(hostAddress_t startLineAddr, const std::pair<hostAddress_t, Cell>& startElement, hostAddress_t prevLineAddr, const std::pair<hostAddress_t, Cell>& prevElement) override;
    status_t appentToNextMatrix(hostAddress_t startLineAddr, const std::pair<hostAddress_t, Cell>& startElement, hostAddress_t prevLineAddr, const std::pair<hostAddress_t, Cell>& prevElement, ConnectMatrix_t& nextMatrix) override;
    RouteTable_t mergeMatrixToRouteTable(const std::vector<ConnectMatrix_t>& matrixVec) override;

    // Config
    int m_maxPathLength;
};

#endif
