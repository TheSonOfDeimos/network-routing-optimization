#include "time.hpp"

Time& Time::instance()
{
    static Time self;
    return self;
}

modelTime_t Time::get()
{
    return m_time;
}

status_t Time::set(modelTime_t time)
{
    std::lock_guard<std::mutex> lock(m_mtx);
    m_timePrev = m_time;
    m_time = time;
    return true;
}

status_t Time::tick()
{
    std::lock_guard<std::mutex> lock(m_mtx);
    m_timePrev = m_time;
    m_time++;
    if (m_timePrev > m_time)
    {
        throw std::overflow_error("Time value overflow");
    }
    return true;
}
