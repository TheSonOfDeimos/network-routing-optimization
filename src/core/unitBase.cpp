#include "unitBase.hpp"

#include "core.hpp"

UnitBase::UnitBase(guid_t guidHint)
{
    if (guidHint != -1)
    {
        m_guid = guidHint;
    }
    else
    {
        static std::atomic<int> guidCounter = 0;
        m_guid = guidCounter++;
    }

    CoreBase::addUnit(this);
}

UnitBase::~UnitBase()
{
    CoreBase::removeUnit(getGuid());
}

modelTime_t UnitBase::getTime()
{
    return Time::instance().get();
}

guid_t UnitBase::getGuid()
{
    return m_guid;
}