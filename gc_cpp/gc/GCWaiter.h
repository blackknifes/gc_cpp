#ifndef __GCWAITER_H__
#define __GCWAITER_H__
#include <Windows.h>
#include "GCLocker.h"

class GCWaiter
{
public:
    GCWaiter();
    ~GCWaiter();
	
    bool wait(GCLocker* pLocker, DWORD timeout = INFINITE) const;
    void notify() const;
    void notifyAll() const;

private:
    mutable CONDITION_VARIABLE m_condition;
};
#endif