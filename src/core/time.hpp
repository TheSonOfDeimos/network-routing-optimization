#ifndef TIME_HPP
#define TIME_HPP

#include <mutex>

#include "types.hpp"

using modelTime_t = int;

class Time
{
public:
    Time() = default;
    Time(const Time&) = delete;
    Time& operator = (Time&) = delete;

    static Time& instance();

    modelTime_t get();
    status_t set(modelTime_t time);
    status_t tick();

private:
    modelTime_t m_time = 0;
    modelTime_t m_timePrev = 0;
    std::mutex m_mtx;
};

#endif