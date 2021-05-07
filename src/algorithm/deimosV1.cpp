#include "deimosV1.hpp"

#include "types.hpp"

DeimosV1::DeimosV1(int maxPathLength, double reqSpeed, double reqPacketloss, double reqPing)
    : m_maxPathLength(maxPathLength),
      m_reqSpeed(reqSpeed),
      m_reqPacketloss(reqPacketloss),
      m_reqPing(reqPing)
{
}

status_t DeimosV1::adoptStartMatrix(ConnectMatrix_t &startMatrix)
{
    status_t status = ERROR_OK;

    for (auto &line : startMatrix)
    {
        for (auto &element : line.second)
        {
            EXIT_IF(element.second.cost != -1, ERROR_LOGIC);
            element.second.cost = 0;
        }
    }

exit:
    return status;
}

status_t DeimosV1::isPathPossible(hostAddress_t startLineAddr, const std::pair<hostAddress_t, Cell> &startElement, hostAddress_t prevLineAddr, const std::pair<hostAddress_t, Cell> &prevElement)
{
    status_t status = ERROR_RESULT_TRUE;

    EXIT_IF(prevLineAddr == prevElement.first, ERROR_RESULT_FALSE);
    EXIT_IF(prevLineAddr == startElement.first && prevElement.first == startLineAddr, ERROR_RESULT_FALSE);
    EXIT_IF(prevElement.second.path.back() != startElement.second.path.front(), ERROR_LOGIC);

    // Check req

    EXIT_IF(prevElement.second.path.size() >= static_cast<std::size_t>(m_maxPathLength), ERROR_RESULT_FALSE);
    {
        double expSpeed = std::min(startElement.second.speed, prevElement.second.speed);
        double expPacketLoss = (1 - ((100 - startElement.second.packetLoss) / 100) * ((100 - prevElement.second.packetLoss) / 100)) * 100;
        double expPing = startElement.second.ping + prevElement.second.ping;
        EXIT_IF(expSpeed < m_reqSpeed, ERROR_RESULT_FALSE);
        EXIT_IF(expPacketLoss > m_reqPacketloss, ERROR_RESULT_FALSE);
        EXIT_IF(expPing > m_reqPing, ERROR_RESULT_FALSE);
    }

    // Find cycles
    {
        std::vector<hostAddress_t> se = startElement.second.path;
        std::vector<hostAddress_t> pe = prevElement.second.path;
        std::sort(se.begin(), se.end());
        std::sort(pe.begin(), pe.end());
        std::vector<hostAddress_t> intersect;
        std::set_intersection(se.begin(), se.end(),
                              pe.begin(), pe.end(),
                              std::back_inserter(intersect));

        EXIT_IF(intersect.size() > 1, ERROR_RESULT_FALSE);
    }

exit:
    return status;
}

status_t DeimosV1::appentToNextMatrix(hostAddress_t startLineAddr, const std::pair<hostAddress_t, Cell> &startElement, hostAddress_t prevLineAddr, const std::pair<hostAddress_t, Cell> &prevElement, ConnectMatrix_t &nextMatrix)
{
    status_t status = ERROR_OK;

    Cell newCell = prevElement.second;
    newCell.path.push_back(startElement.first);
    newCell.speed = std::min(startElement.second.speed, prevElement.second.speed);
    newCell.packetLoss = (1 - ((100 - startElement.second.packetLoss) / 100) * ((100 - prevElement.second.packetLoss) / 100)) * 100;
    newCell.ping = startElement.second.ping + prevElement.second.ping;

    std::vector<double> weightVector = {};
    weightVector.push_back((1 / newCell.speed) / (1 / m_reqSpeed));
    weightVector.push_back(newCell.packetLoss / m_reqPacketloss);
    weightVector.push_back(newCell.ping / m_reqPing);
    std::sort(weightVector.begin(), weightVector.end(), std::greater<double>());
    newCell.cost = prevElement.second.cost + weightVector.front();

    nextMatrix[prevLineAddr][startElement.first] = newCell;

exit:
    return status;
}

RouteTable_t DeimosV1::mergeMatrixToRouteTable(const std::vector<ConnectMatrix_t> &matrixVec)
{
    RouteTable_t table;

    for (auto& matrix : matrixVec)
    {
        for (auto& line : matrix)
        {
            for (auto& elem : line.second)
            {
                table.push_back({});
                table.back().cost = elem.second.cost;
                table.back().source = line.first;
                table.back().destination = elem.first;
                table.back().path = elem.second.path;
            }
        }
    }

    std::sort(table.begin(), table.end(), [](Route& cell_1, Route& cell_2)
    {
        return cell_1.cost < cell_2.cost;
    });


    return table;
}
