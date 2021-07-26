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
    bool m_verbose;                        //�Ƿ�򿪵���
    std::chrono::milliseconds m_gcPeriod;  // gc����
    size_t m_edenThreshold;                //�������ֵ
    size_t m_survisorThreshold;            //�������ֵ
    size_t m_evolutionAge;                 //�������������
    size_t m_markThreadParallelCount;      // mark�̲߳�����
    size_t m_sweepThreadParallelCount;     // sweep�̲߳�����
};
#endif