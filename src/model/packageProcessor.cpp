#include "packageProcessor.hpp"

PackageProcessor::PackageProcessor(double bandwidth)
    : m_bandwidth(bandwidth / 1e6)
{
}

status_t PackageProcessor::push(packagePtr_t pack)
{
    status_t status = ERROR_OK;

    EXIT_IF(pack == nullptr, ERROR_NO_EFFECT);

    {
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
    if (m_currentPackage == nullptr)
    {
        return nullptr;
    }
    return std::move(m_currentPackage);
}
