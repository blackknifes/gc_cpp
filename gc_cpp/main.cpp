#include <Windows.h>
#include <conio.h>
#include <math.h>
#include <stdint.h>

#include <limits>
#include <unordered_set>

#include "gc/GCManager.h"
#include "gc/GCPtr.h"

#define TEST_COUNT 512

GCLocker locker;
std::unordered_set<DWORD> threads;
std::atomic<uint32_t> s_count = 0;
bool runFlag = true;

class Test;

class TestChild : public GarbageCollected<TestChild>
{
public:
    TestChild(GCPtr<Test> pTest);
    ~TestChild();

    void gcTrace(GCVisitor* visitor) override;

private:
    GCPtr<Test> m_child;
};

class Test : public GarbageCollected<Test>
{
public:
    Test()
    {
        uint32_t count = ++s_count;
        if (count % 2048 == 0) printf("object count: %u\n", count);
    }
    ~Test()
    {
        --s_count;
    }

    std::vector<GCPtr<TestChild>> m_children;

    void gcTrace(GCVisitor* visitor) override
    {
        visitor->visit(m_children);
    }
};

TestChild::TestChild(GCPtr<Test> pTest) : m_child(pTest)
{
    uint32_t count = ++s_count;
    if (count % 2048 == 0) printf("object count: %u\n", count);
}

TestChild::~TestChild()
{
    --s_count;
}

void TestChild::gcTrace(GCVisitor* visitor)
{
    visitor->visit(m_child);
}

GCPtr<Test> s_test;

DWORD CALLBACK ThreadProc(LPVOID)
{
    GCThreadState state;

    size_t i = 0;
    while (runFlag)
    {
        locker.lock();
        if (!s_test) s_test = new Test();
        {
            GCUnsafeScope scope;
            s_test->m_children.push_back(new TestChild(s_test));
        }
        locker.unlock();
    }

    return 0;
}

struct _NT_TIB* GetTib()
{
#ifdef _WIN64
    return (struct _NT_TIB*)__readgsqword(FIELD_OFFSET(NT_TIB, Self));
#else
    return (struct _NT_TIB*)(ULONG_PTR)__readfsdword(PcTeb);
#endif
};

int main()
{
    NT_TIB* pTib = GetTib();
    int a = 0;
    void* d = &a;
    GCManager manager;
    HANDLE hThreads[TEST_COUNT];

    printf("∆Ù∂Ø≤‚ ‘\n");
    for (size_t i = 0; i < TEST_COUNT; ++i)
        hThreads[i] = CreateThread(nullptr, 10240, ThreadProc, nullptr, 0, nullptr);

    for (size_t i = 0; i < 10; ++i)
    {
        manager.stopWorld();
        manager.markSweep();
        manager.resumeWorld();
        printf("%u\n", s_count.load());
        Sleep(500);
    }
    runFlag = false;
    for (size_t i = 0; i < TEST_COUNT; ++i)
    {
        if (WaitForSingleObject(hThreads[i], INFINITE) != WAIT_OBJECT_0) printf("wait fail\n");
    }
    manager.stopWorld();
    manager.markSweep();
    manager.resumeWorld();
    printf("%u\n", s_count.load());

    return 0;
}
