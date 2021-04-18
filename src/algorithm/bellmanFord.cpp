#include "bellmanFord.hpp"

#include <unordered_map>
#include <vector>

#include "types.hpp"

status_t BellmanFord::adoptStartMatrix(ConnectMatrix_t &startMatrix)
{
    status_t status = ERROR_OK;

    for (auto &line : startMatrix)
    {
        for (auto &element : line.second)
        {
            EXIT_IF(element.second.cost != -1, ERROR_LOGIC);
            element.second.cost = 1;
        }
    }

exit:
    return status;
}

status_t BellmanFord::isPathPossible(hostAddress_t startLineAddr, const std::pair<hostAddress_t, Cell> &startElement, hostAddress_t prevLineAddr, const std::pair<hostAddress_t, Cell> &prevElement)
{
    status_t status = ERROR_RESULT_TRUE;

    EXIT_IF(prevLineAddr == prevElement.first, ERROR_RESULT_FALSE);
    EXIT_IF(prevLineAddr == startElement.first && prevElement.first == startLineAddr, ERROR_RESULT_FALSE);
    EXIT_IF(prevElement.second.path.back() != startElement.second.path.front(), ERROR_LOGIC);

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

status_t BellmanFord::appentToNextMatrix(hostAddress_t startLineAddr, const std::pair<hostAddress_t, Cell>& startElement, hostAddress_t prevLineAddr, const std::pair<hostAddress_t, Cell>& prevElement, ConnectMatrix_t& nextMatrix)
{
    status_t status = ERROR_OK;

    Cell newCell = prevElement.second;
    newCell.path.push_back(startElement.first);
    newCell.cost++;
    nextMatrix[prevLineAddr][startElement.first] = newCell;

exit:
    return status;
}

RouteTable_t BellmanFord::mergeMatrixToRouteTable(const std::vector<ConnectMatrix_t> &matrixVec)
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

    // for (auto& line : matrixVec.back())
    // {
    //     for (auto& element : line.second)
    //     {
    //         double minCost = element.second.cost;
    //         auto bestCell = element.second;
    //         for (auto& matrix : matrixVec)
    //         {
    //             if (matrix.find(line.first) == matrix.end() || matrix.at(line.first).find(element.first) == matrix.at(line.first).end())
    //             {
    //                 continue;
    //             }
    //             auto newCell = matrix.at(line.first).at(element.first);
    //             if (minCost > newCell.cost)
    //             {
    //                 minCost = newCell.cost;
    //                 bestCell = newCell;
    //             }
    //         }
    //         table.push_back({});
    //         table.back().cost = bestCell.cost;
    //         table.back().source = bestCell.path.back();
    //         table.back().destination = bestCell.path.front();
    //         table.back().path = bestCell.path;
    //     }
    // }

    return table;
}
