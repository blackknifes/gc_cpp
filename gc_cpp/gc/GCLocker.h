#ifndef __GCLOCKER_H__
#define __GCLOCKER_H__
#include <Windows.h>

class GCLocker
{
public:
    GCLocker();
    ~GCLocker();

    void lock() const;
    void unlock() const;

private:
    friend class GCWaiter;
    mutable CRITICAL_SECTION m_section;
};
#endif