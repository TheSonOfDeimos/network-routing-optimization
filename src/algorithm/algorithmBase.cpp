#include "algorithmBase.hpp"

#include <vector>

RouteTable_t AlgorithmBase::buildRoutes(const ConnectMatrix_t &matrix)
{
    std::vector<ConnectMatrix_t> matrixVec = {matrix};
    adoptStartMatrix(matrixVec.back());

    while (matrixVec.back().size() != 0)
    {
        matrixVec.emplace_back(buildNext(matrixVec.front(), matrixVec.back()));
    }
    matrixVec.pop_back();

    return mergeMatrixToRouteTable(matrixVec);
}

ConnectMatrix_t AlgorithmBase::buildNext(const ConnectMatrix_t &startMatrix, const ConnectMatrix_t &prevMatrix)
{
    status_t status = ERROR_OK;

    ConnectMatrix_t nextMatrix;

    for (auto &prevLine : prevMatrix)
    {
        for (auto &prevElement : prevLine.second)
        {
            hostAddress_t startLineAddr = prevElement.first;
            for (auto &startElement : startMatrix.at(prevElement.first))
            {
                hostAddress_t prevLineAddr = prevLine.first;
                if (IS_SUCCESS(isPathPossible(startLineAddr, startElement, prevLineAddr, prevElement)))
                {
                    RUN(appentToNextMatrix(startLineAddr, startElement, prevLineAddr, prevElement, nextMatrix));
                }
            }
        }
    }
exit:
    return nextMatrix;
}
