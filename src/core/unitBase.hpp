#ifndef UNIT_BASE_HPP
#define UNIT_BASE_HPP

#include <mutex>
#include <atomic>

#include "types.hpp"
#include "time.hpp"

class UnitBase
{
public:
    UnitBase(guid_t guidHint = -1);

    virtual ~UnitBase();
    virtual status_t update() = 0;
    modelTime_t getTime();
    guid_t getGuid();

private:
    guid_t m_guid;
};

#endif