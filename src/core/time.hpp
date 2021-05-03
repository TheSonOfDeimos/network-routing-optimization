#ifndef TIME_HPP
#define TIME_HPP

#include <mutex>
#include <vector>

#include "types.hpp"

// In picoseconds
using modelTime_t = __uint128_t;

static const modelTime_t g_onePicosecond = 1;
static const modelTime_t g_oneNanosecond = 1e3;
static const modelTime_t g_oneMicrosecond = 1e6;
static const modelTime_t g_oneMillisecond = 1e9;
static const modelTime_t g_oneSecond = 1e12;

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
    modelTime_t m_timeStep = 100000;
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
    std::mutex m_mtx;
};

class Timer
{
public:
    modelTime_t setTimer(modelTime_t time);
    bool isTimerElapsed();

public:
    modelTime_t m_startTime = 0;
    modelTime_t m_alarmTime = 0;
    std::mutex m_mtx;
};

#endif
