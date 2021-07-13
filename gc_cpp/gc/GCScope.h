// #ifndef __GCSCOPE_H__
// #define __GCSCOPE_H__
// #include <vector>
// #include "GarbageCollected.h"
// 
// //gc¸ùÓò
// class GCScope
// {
// public:
//     static GCScope* GetCurrent();
//     GCScope();
//     ~GCScope();
// 
//     GCScope* next() const;
//     GCScope* pre() const;
// 
//     void getAll(std::vector<GarbageCollection*>& roots) const;
// 
//     void addGarbage(GarbageCollection* pGarbage);
// 
// private:
//     friend class GCThreadState;
//     friend class GCManager;
// 
//     GCScope* m_pre;
//     GCScope* m_next;
//     std::vector<GarbageCollection*> m_garbageList;
// };
// #endif