#ifndef CORE_HPP
#define CORE_HPP

#include <map>

#include "types.hpp"

class UnitBase;

class CoreBase
{
public :
    virtual ~CoreBase() = default;

    virtual status_t start() = 0;
    virtual status_t stop() = 0;

    // Unit usage
    UnitBase* getUnit(guid_t guid);
    
    using iter_t = std::map<guid_t, UnitBase*>::iterator;
    iter_t beginUnit();
    iter_t endUnit();

private:
    friend UnitBase;
    static status_t addUnit(UnitBase* unit);
    static status_t removeUnit(guid_t guid);

    static std::map<guid_t, UnitBase*> m_unitMap;
};

#endif
