#ifndef __GCVISITOR_H__
#define __GCVISITOR_H__

template<typename _Ty>
class GCPtr;

class GarbageCollection;

class GCVisitor
{
public:
    virtual void visit(GarbageCollection* pGarbage);

    template<typename _Ty>
    void visit(const GCPtr<_Ty>& pGarbage)
    {
        visit(pGarbage.get());
    }
};
#endif