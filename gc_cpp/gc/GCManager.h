#ifndef __GCMANAGER_H__
#define __GCMANAGER_H__
#include <unordered_map>
#include <vector>

#include "GCLocker.h"
#include "GCThreadState.h"
#include "GarbageCollected.h"

class GCManager
{
public:
    static GCManager* GetGlobal();

    GCManager();
    ~GCManager();

    GCThreadState* getThreadState(DWORD id) const;

    void addThreadState(GCThreadState* state);
    void removeThreadState(GCThreadState* state);

    //������ͣ
    void stopWorld();
    //����ָ�
    void resumeWorld();

    //��ӡ��ǰ���밲ȫ���̵߳�����
    void printNumberOfSafePointThread() const;

    void markSweep();

private:
    GCLocker m_threadsLocker;
    std::unordered_map<DWORD, GCThreadState*> m_threads;  // gc�߳�
    std::list<GarbageCollection*> m_garbages;
    std::vector<GarbageCollection*> m_delayCollected;
    GCLocker m_gcLocker;
    GCWaiter m_gcWaiter;
};
#endif