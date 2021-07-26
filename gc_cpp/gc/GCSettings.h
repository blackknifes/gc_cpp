#ifndef __GCSETTINGS_H__
#define __GCSETTINGS_H__
#include <chrono>

class GCSettings
{
public:
    GCSettings();

    void setGCPeriod(int64_t time);
    int64_t getGCPeriod() const;

    size_t getEdenThreshold() const;
    void setEdenThreshold(size_t val);

    size_t getEvolutionAge() const;
    void setEvolutionAge(size_t val);

    bool isVerbose() const;
    void setVerbose(bool val);

    size_t getMarkThreadParallelCount() const;
    void setMarkThreadParallelCount(size_t val);

    size_t getSweepThreadParallelCount() const;
    void setSweepThreadParallelCount(size_t val);

    size_t getSurvisorThreshold() const;
    void setSurvisorThreshold(size_t val);

private:
    bool m_verbose;                        //是否打开调试
    std::chrono::milliseconds m_gcPeriod;  // gc周期
    size_t m_edenThreshold;                //年轻代阈值
    size_t m_survisorThreshold;            //中年代阈值
    size_t m_evolutionAge;                 //中年代晋升年龄
    size_t m_markThreadParallelCount;      // mark线程并行数
    size_t m_sweepThreadParallelCount;     // sweep线程并行数
};
#endif