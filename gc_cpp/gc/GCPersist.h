#ifndef __GCPERSIST_H__
#define __GCPERSIST_H__
#include "GCThreadState.h"

template<typename _Ty>
class GCPersist
{
public:
    GCPersist() : m_pointer(nullptr)
    {
        addRoot();
    };

    GCPersist(_Ty* ptr) : m_pointer(nullptr)
    {
        addRoot();
        GCUnsafeScope unsafeScope;
        m_pointer = ptr;
    }

    GCPersist(const GCPersist& ptr) : m_pointer(nullptr)
    {
        addRoot();
        GCUnsafeScope unsafeScope;
        m_pointer = ptr.m_pointer;
    }

    template<typename _Ty2>
    GCPersist(const GCPersist<_Ty2>& ptr) : m_pointer(nullptr)
    {
        addRoot();
        GCUnsafeScope unsafeScope;
        m_pointer = ptr.m_pointer;
    }

    const GCPersist& operator=(_Ty* ptr)
    {
        GCUnsafeScope unsafeScope;
        m_pointer = ptr;
        return *this;
    }

    const GCPersist& operator=(const GCPersist& ptr)
    {
        GCUnsafeScope unsafeScope;
        m_pointer = ptr.m_pointer;
        return *this;
    }

    template<typename _Ty2>
    const GCPersist& operator=(const GCPersist<_Ty2>& ptr)
    {
        GCUnsafeScope unsafeScope;
        m_pointer = ptr.m_pointer;
        return *this;
    }

    bool operator!() const
    {
        return !m_pointer;
    }

    operator bool() const
    {
        return m_pointer;
    }

    bool operator==(const _Ty* ptr) const
    {
        return m_pointer == ptr;
    }
    bool operator==(const GCPersist& ptr) const
    {
        return m_pointer == ptr.m_pointer;
    }
    template<typename _Ty2>
    bool operator==(const GCPersist<_Ty2>& ptr) const
    {
        return m_pointer == ptr.m_pointer;
    }

    bool operator!=(const _Ty* ptr) const
    {
        return m_pointer != ptr;
    }
    bool operator!=(const GCPersist& ptr) const
    {
        return m_pointer != ptr.m_pointer;
    }
    template<typename _Ty2>
    bool operator!=(const GCPersist<_Ty2>& ptr) const
    {
        return m_pointer != ptr.m_pointer;
    }

    _Ty* operator->() const
    {
        return m_pointer;
    }

    _Ty& operator*() const
    {
        return *m_pointer;
    }

    _Ty* get() const
    {
        return m_pointer;
    }

    operator _Ty*() const 
    {
        return m_pointer;
    }
private:
    static GarbageCollected* Cast(void* ptr)
    {
        return static_cast<GarbageCollected*>((_Ty*)ptr);
    }

    void addRoot()
    {
        GCThreadState* pThreadState = GCThreadState::GetCurrent();
        if (pThreadState)
            pThreadState->addRoot((void**)&m_pointer, Cast);
        else
            GCManager::GetGlobal()->addRoot((void**)&m_pointer, Cast);
    }

    void removeRoot() 
    {
        GCThreadState* pThreadState = GCThreadState::GetCurrent();
        if (pThreadState)
            pThreadState->popRoot();
        else
            GCManager::GetGlobal()->removeRoot((void**)&m_pointer);
    }

    _Ty* m_pointer;
};
#endif