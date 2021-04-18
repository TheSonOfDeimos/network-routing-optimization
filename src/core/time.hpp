#ifndef TIME_HPP
#define TIME_HPP

#include <mutex>
#include <vector>

#include "types.hpp"

// In picoseconds
using modelTime_t = __uint128_t;

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
    modelTime_t m_timeStep = 10;
    std::mutex m_mtx;
};

class Stopwatch
{
public:
    // Returns start time 
    modelTime_t start();

    // Returns diff nowTime - prevLapTime
    modelTime_t lap();

    // Returns stopTime
    modelTime_t stop();

public:
    modelTime_t m_startTime = 0;
    std::vector<modelTime_t> m_lapVec = {};
    modelTime_t m_stopTime = 0;
};

class Timer
{
public:
    modelTime_t setTimer(modelTime_t time);
    bool isTimerElapsed();

public:
    modelTime_t m_startTime = 0;
    modelTime_t m_alarmTime = 0;
};

#endif