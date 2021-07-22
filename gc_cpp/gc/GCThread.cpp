#include "GCThread.h"

namespace
{
    DWORD CALLBACK ThreadProc(GCThread* pThread)
    {
        pThread->run();
        return 0;
    }
}  // namespace

bool GCThread::JoinAll(GCThread** pThreadArray, size_t count, DWORD overtime /*= INFINITE*/)
{
    HANDLE hThreads[MAXIMUM_WAIT_OBJECTS];
    size_t index = 0;
    while (count != 0)
    {
        DWORD checkCount = (DWORD)(count > MAXIMUM_WAIT_OBJECTS ? MAXIMUM_WAIT_OBJECTS : count);
        count -= checkCount;
        for (size_t i = 0; i < checkCount; ++i) hThreads[i] = pThreadArray[index++]->m_hThread;
        if (WaitForMultipleObjects(checkCount, hThreads, TRUE, overtime) != WAIT_OBJECT_0)
            return false;
    }
    return true;
}

GCThread::GCThread() : m_hThread(nullptr) {}

GCThread::~GCThread()
{
    if (m_hThread) CloseHandle(m_hThread);
}

void GCThread::start(size_t stackSize /*= 0*/)
{
    m_hThread =
        CreateThread(nullptr, stackSize, (LPTHREAD_START_ROUTINE)ThreadProc, this, 0, nullptr);
}

void GCThread::stop()
{
    ::TerminateThread(m_hThread, -1);
}

bool GCThread::join(DWORD overtime /*= INFINITE*/)
{
    return ::WaitForSingleObject(m_hThread, overtime) == WAIT_OBJECT_0;
}
