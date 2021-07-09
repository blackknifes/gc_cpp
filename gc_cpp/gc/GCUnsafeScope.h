#ifndef __GCUNSAFESCOPE_H__
#define __GCUNSAFESCOPE_H__
class GCThreadState;

class GCUnsafeScope
{
public:
    GCUnsafeScope();
    ~GCUnsafeScope();

private:
    GCThreadState* m_threadState;
};
#endif