#ifndef DEIMOSV1_HPP
#define DEIMOSV1_HPP

#include "algorithmBase.hpp"

class DeimosV1 : public AlgorithmBase
{
public:
    DeimosV1(int maxPathLength, double reqSpeed, double reqPacketloss, double reqPing);

private:
    status_t adoptStartMatrix(ConnectMatrix_t& startMatrix) override;
    status_t isPathPossible(hostAddress_t startLineAddr, const std::pair<hostAddress_t, Cell>& startElement, hostAddress_t prevLineAddr, const std::pair<hostAddress_t, Cell>& prevElement) override;
    status_t appentToNextMatrix(hostAddress_t startLineAddr, const std::pair<hostAddress_t, Cell>& startElement, hostAddress_t prevLineAddr, const std::pair<hostAddress_t, Cell>& prevElement, ConnectMatrix_t& nextMatrix) override;
    RouteTable_t mergeMatrixToRouteTable(const std::vector<ConnectMatrix_t>& matrixVec) override;

    // Config
    int m_maxPathLength;
    double m_reqSpeed;
    double m_reqPacketloss;
    double m_reqPing;
};

#endif
