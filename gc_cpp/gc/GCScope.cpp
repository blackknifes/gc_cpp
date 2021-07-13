// #include "GCScope.h"
// 
// #include "GCThreadState.h"
// #include "GCUnsafeScope.h"
// 
// GCScope* GCScope::GetCurrent()
// {
//     return GCThreadState::GetCurrent()->getScope();
// }
// 
// GCScope::GCScope()
// {
//     GCThreadState* pThreadState = GCThreadState::GetCurrent();
//     GCUnsafeScope unsafeScope;
//     m_pre = pThreadState->getScope();
//     m_next = nullptr;
//     if (m_pre)
//     {
//         m_pre->m_next = this;
//         pThreadState->m_lastScope = this;
//     }
// }
// 
// GCScope::~GCScope()
// {
//     GCThreadState* pThreadState = GCThreadState::GetCurrent();
//     GCUnsafeScope unsafeScope;
//     pThreadState->m_lastScope = m_pre;
//     if (m_pre) m_pre->m_next = nullptr;
// }
// 
// GCScope* GCScope::next() const
// {
//     return m_next;
// }
// 
// GCScope* GCScope::pre() const
// {
//     return m_pre;
// }
// 
// void GCScope::getAll(std::vector<GarbageCollection*>& roots) const
// {
//     roots.insert(roots.end(), m_garbageList.begin(), m_garbageList.end());
// }
// 
// void GCScope::addGarbage(GarbageCollection* pGarbage)
// {
//     m_garbageList.push_back(pGarbage);
// }