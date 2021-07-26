#include "GCTimer.h"

GCTimer::GCTimer() : m_time(std::chrono::system_clock::now()) {}

void GCTimer::update()
{
    m_time = std::chrono::system_clock::now();
}

std::chrono::system_clock::duration GCTimer::getDuration() const
{
    return std::chrono::system_clock::now() - m_time;
}

int64_t GCTimer::getMilliSeconds() const
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(getDuration()).count();
}
