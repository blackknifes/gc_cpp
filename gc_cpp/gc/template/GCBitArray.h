#ifndef __GCBITARRAY_H__
#define __GCBITARRAY_H__

class GCBitArray
{
public:
    static constexpr const size_t kUnitBit = sizeof(size_t) * 8;
    static constexpr const size_t kOffsetBit = sizeof(size_t) == 8 ? 6 : 5;

    GCBitArray(size_t bitCount);
    ~GCBitArray();

    void setFlag(size_t offset, bool val);
    bool isFlag(size_t offset);

    size_t searchNextFlag(size_t offset = 0) const;

    void clear();

private:
    size_t getCountOfSizeT() const;

    size_t* m_bitArray;
    size_t m_bitCount;
};
#endif