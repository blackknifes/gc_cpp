#ifndef __GCTIMER_H__
#define __GCTIMER_H__
#include <chrono>

class GCTimer
{
public:
    GCTimer();
    
    void update();
    std::chrono::system_clock::duration getDuration() const;
    int64_t getMilliSeconds() const;
private:
    std::chrono::system_clock::time_point m_time;
};
#endif