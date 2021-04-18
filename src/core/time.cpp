#include "time.hpp"

#include <vector>

Time &Time::instance()
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
    m_time += m_timeStep;
    if (m_timePrev > m_time)
    {
        throw std::overflow_error("Time value overflow");
    }
    return true;
}

// Returns start time
modelTime_t Stopwatch::start()
{
    m_startTime = Time::instance().get();
    m_lapVec.push_back(m_startTime);
    return m_startTime;
}

// Returns diff nowTime - prevLapTime
modelTime_t Stopwatch::lap()
{
    m_lapVec.push_back(Time::instance().get());
    return *m_lapVec.rbegin() - *(m_lapVec.rbegin() + 1);
}

// Returns stopTime
modelTime_t Stopwatch::stop()
{
    m_stopTime = Time::instance().get();
    m_lapVec.push_back(m_stopTime);
    return m_stopTime;
}

modelTime_t Timer::setTimer(modelTime_t time)
{
    m_startTime = Time::instance().get();
    m_alarmTime = m_startTime + time;
    return m_startTime;
}

bool Timer::isTimerElapsed()
{
    return m_alarmTime <= Time::instance().get();
}
