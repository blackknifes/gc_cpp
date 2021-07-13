#ifndef __GCMANAGER_H__
#define __GCMANAGER_H__
#include <unordered_map>
#include <vector>

#include "GCLocker.h"
#include "GCStopFlag.h"
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

    const GCStopFlag* getStopFlag() const;

    //ɨ����ڵ�
    void scanRoots();

private:
    GCLocker m_threadsLocker;
    GCStopFlag m_stopFlag;
    std::unordered_map<DWORD, GCThreadState*> m_threads;  // gc�߳�
    std::list<GarbageCollection*> m_garbages;
    std::vector<GarbageCollection*> m_delayCollected;
};
#endif