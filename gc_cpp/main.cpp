#include <Windows.h>
#include <assert.h>
#include <conio.h>
#include <math.h>
#include <stdint.h>

#include <limits>
#include <unordered_set>

#include "gc/GCManager.h"
#include "gc/GCPersist.h"
#include "gc/GCPtr.h"
#include "gc/GCScope.h"
#include "gc/template/GCBitArray.h"
#include "gc/platform/GCPlatformAPI.h"
#include <thread>

#define TEST_COUNT 512

GCLocker locker;
std::unordered_set<DWORD> threads;
std::atomic<uint32_t> s_count = 0;
std::atomic<uint32_t> s_threadCount = 0;
bool runFlag = true;

class Test;

class TestChild : public GarbageCollected
{
public:
    TestChild(GCPtr<Test> pTest);
    ~TestChild();

    void gcTrace(GCVisitor* visitor) override;

private:
    GCPtr<Test> m_child;
};

class Test : public GarbageCollected
{
public:
    Test()
    {
        uint32_t count = ++s_count;
        // if (count % 2048 == 0) printf("object count: %u\n", count);
    }
    ~Test()
    {
        if (--s_count == 0) printf("all clean *******************************\n");
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
    // if (count % 2048 == 0) printf("object count: %u\n", count);
}

TestChild::~TestChild()
{
    if (--s_count == 0) printf("all clean *******************************\n");
}

void TestChild::gcTrace(GCVisitor* visitor)
{
    visitor->visit(m_child);
}

DWORD CALLBACK ThreadProc(LPVOID ptr)
{
    GCPersist<Test>& s_test = *(GCPersist<Test>*)ptr;
    GCThreadState state;

    size_t i = 0;
    while (runFlag)
    {
        GCScope scope;
        locker.lock();
        if (!s_test) s_test = new Test();
        {
            GCUnsafeScope unsafeScope;
            s_test->m_children.push_back(new TestChild(s_test));
        }
        if (s_test->m_children.size() % 10240 == 0) s_test = nullptr;
        locker.unlock();
    }

    return 0;
}

void test()
{
    runFlag = true;
    GCManager manager;
    auto pSettings = manager.getSettings();
    pSettings->setVerbose(true);
    pSettings->setGCPeriod(30000);
    pSettings->setEdenThreshold(0x10000);
    pSettings->setSurvisorThreshold(32 * 1024 * 1024);

    GCPersist<Test> test;
    HANDLE hThreads[TEST_COUNT];

    printf("∆Ù∂Ø≤‚ ‘\n");
    for (size_t i = 0; i < TEST_COUNT; ++i)
        hThreads[i] = CreateThread(nullptr, 10240, ThreadProc, &test, 0, nullptr);

    Sleep(10000);
    runFlag = false;
    for (size_t i = 0; i < TEST_COUNT; ++i)
    {
        WaitForSingleObject(hThreads[i], INFINITE);
        CloseHandle(hThreads[i]);
    }
    test = nullptr;
}

void test2()
{
    GCPlatformAPI::InitMainThread();

    assert(GCPlatformAPI::IsMainThread());
    std::thread tmpThread([]() 
    {
        assert(!GCPlatformAPI::IsMainThread());
    });
    tmpThread.join();

    GCBitArray bitArr(4096);
    bitArr.setFlag(64, true);
    bitArr.setFlag(66, true);
    size_t val = bitArr.searchNextFlag();
    assert(val == 64);
    val = bitArr.searchNextFlag(65);
    assert(val == 66);
    val = bitArr.searchNextFlag(67);
    assert(val == GCBitArray::npos);

    uint64_t utfTime = GCPlatformAPI::CurrentUTFTime();
    PlatformTime platformTime = GCPlatformAPI::ConvertToPlatformTime(utfTime);
    uint64_t localTime = GCPlatformAPI::UTFToLocalTime(utfTime);
    PlatformTime localPlatTime = GCPlatformAPI::ConvertToPlatformTime(localTime);
    char buf[1024];
    GCPlatformAPI::FormatNormalTime(localPlatTime, buf, 1024);
    printf("%s\n", buf);
}

int main()
{
    test2();
    return 0;
    for (size_t i = 0; i < 10; ++i)
    {
        system("cls");
        test();
        printf("count: %u\n", s_count.load());
        if (s_count != 0) _CrtDbgBreak();
    }

    return 0;
}
