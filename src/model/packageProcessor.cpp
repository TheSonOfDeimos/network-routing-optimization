#include "packageProcessor.hpp"

#include <numeric>

PackageProcessor::PackageProcessor(double bandwidth)
    : m_bandwidth(bandwidth / 1e6)
{
}

status_t PackageProcessor::push(packagePtr_t pack)
{
    status_t status = ERROR_OK;
    std::lock_guard<std::mutex> lock(m_mtx);

    EXIT_IF(pack == nullptr, ERROR_NO_EFFECT);

    {
        pack->inProcess = Time::instance().get();
        m_currentPackage.reset(nullptr);
        m_currentPackage = std::move(pack);

        double vol = m_currentPackage->volume * 8; // bytes * 8bit = bits
        m_processingTime = vol / m_bandwidth; // bits / (Bits/picosec)
        m_timer.setTimer(m_processingTime);
    }
exit:
    return status;
}

bool PackageProcessor::isReady()
{
    return m_timer.isTimerElapsed();
}

packagePtr_t PackageProcessor::pop()
{   
    std::lock_guard<std::mutex> lock(m_mtx);
    if (m_currentPackage == nullptr)
    {
        return nullptr;
    }

    m_currentPackage->outProcess = Time::instance().get();    
    m_speeds.push_back(((m_currentPackage->volume * 8) / (double)(m_currentPackage->outProcess - m_currentPackage->inQueue)) * 1e6);
    if (m_speeds.size() > (std::size_t)m_measureFrame) m_speeds.erase(m_speeds.begin());
    return std::move(m_currentPackage);
}

double PackageProcessor::getSpeed()
{
    std::lock_guard<std::mutex> lock(m_mtx);
    if (m_speeds.size() == 0) return m_bandwidth;
    auto speed = std::accumulate(m_speeds.begin(), m_speeds.end(), 0.0, std::plus<double>());
    speed /= m_speeds.size();
    return speed;
}
