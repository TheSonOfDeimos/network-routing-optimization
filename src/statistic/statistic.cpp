#include "statistic.hpp"


Statistic& Statistic::instance()
{
    static Statistic self;
    return self;
}

status_t Statistic::report(const NodeCharacteristics &params)
{
    return 0;
}

status_t Statistic::report(packagePtr_t package)
{
    return 0;
}

