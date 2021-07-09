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

    GCPtr<TestChild> m_child;

    void gcTrace(GCVisitor* visitor) override
    {
        visitor->visit(m_child);
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
    while (true)
    {
        GCScope scope;
        if (!s_test)
        {
            locker.lock();
            s_test = new Test();
            s_test->m_child = new TestChild(s_test);
            locker.unlock();
        }
        else
        {
            locker.lock();
            s_test = nullptr;
            locker.unlock();
        }
        Sleep(500);
    }

    return 0;
}

int main()
{
    GCManager manager;
    HANDLE hThreads[TEST_COUNT];

    printf("∆Ù∂Ø≤‚ ‘\n");
    for (size_t i = 0; i < TEST_COUNT; ++i)
        hThreads[i] = CreateThread(nullptr, 10240, ThreadProc, nullptr, 0, nullptr);

    while (true)
    {
        manager.stopWorld();
        manager.markSweep();
        manager.resumeWorld();
        printf("%u\n", s_count.load());
        Sleep(5000);
    }

    return 0;
}
