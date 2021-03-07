#include "core.hpp"

#include "unitBase.hpp"

std::map<guid_t, UnitBase *> CoreBase::m_unitMap = {};

CoreBase::iter_t CoreBase::beginUnit()
{
    return m_unitMap.begin();
}

CoreBase::iter_t CoreBase::endUnit()
{
    return m_unitMap.end();
}

UnitBase *CoreBase::getUnit(guid_t guid)
{
    auto found = m_unitMap.find(guid);
    if (found == m_unitMap.end())
    {
        return nullptr;
    }
    return found->second;
}

status_t CoreBase::addUnit(UnitBase *unit)
{
    auto found = m_unitMap.find(unit->getGuid());
    if (found == m_unitMap.end())
    {
        m_unitMap.insert({unit->getGuid(), unit});
        return 0;
    }
    return -1;
}

status_t CoreBase::removeUnit(guid_t guid_t)
{
    return m_unitMap.erase(guid_t) >= 1 ? 0 : -1;
}