#ifndef __GCTHREAD_H__
#define __GCTHREAD_H__
#include <Windows.h>

class GCThread
{
public:
    static bool JoinAll(GCThread** pThreadArray, size_t count, DWORD overtime = INFINITE);

    GCThread();
    ~GCThread();

    void start(size_t stackSize = 0);
    void stop();
    bool join(DWORD overtime = INFINITE);

    virtual void run() = 0;
private:
    HANDLE m_hThread;
};
#endif