#ifndef __GCMANAGER_H__
#define __GCMANAGER_H__
#include <unordered_map>
#include <vector>

#include "GCLocker.h"
#include "GCStopTheWorld.h"
#include "GCThreadState.h"
#include "GarbageCollected.h"

class GCMarkThread;
class GCSweepThread;

class GCManager
{
public:
    static GCManager* GetGlobal();

    GCManager();
    ~GCManager();

    GCThreadState* getThreadState(DWORD id) const;

    void addThreadState(GCThreadState* state);
    void removeThreadState(GCThreadState* state);

    void gc();

    //������ͣ
    void stopWorld();
    //����ָ�
    void resumeWorld();
    //���
    void mark();
    //������
    void lazySweep();

    //�ȴ�����gc����
    void waitFinish();

    //��ӡ��ǰ���밲ȫ���̵߳�����
    void printNumberOfSafePointThread() const;

    const GCStopTheWorld* getStopFlag() const;


    void stopMarkThread();
    void stopSweepThread();

private:
    template<typename _Ty>
    friend class GCPersist;

    typedef GarbageCollected* (*PFN_Cast)(void*);
    void addRoot(void** ppAddress, PFN_Cast cast);
    void removeRoot(void** ppAddress);

    GCLocker m_locker;          //��
    GCStopTheWorld m_stopTheWorld;  //ֹͣ��

    GCMarkThread* m_markThread;    // mark�߳�
    GCSweepThread* m_sweepThread;  // lazy sweep �߳�

    std::unordered_map<void**, PFN_Cast> m_roots;         //ȫ�ָ��ڵ�
    std::unordered_map<DWORD, GCThreadState*> m_threads;  // gc�߳�
    std::list<GarbageCollected*> m_garbages;              //ȫ��������
    std::vector<GarbageCollected*> m_willLazySweep;       //�ӳ������
};
#endif