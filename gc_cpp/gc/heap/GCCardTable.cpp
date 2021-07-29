#include "GCCardTable.h"

GCCardTable::GCCardTable(size_t cardSize) : m_cardSize(cardSize) {}

void GCCardTable::setDirty(GCAddress addr) {}

void GCCardTable::setDirty(size_t index) {}

void GCCardTable::isDirty(GCAddress addr) const {}

void GCCardTable::isDirty(size_t index) const {}

void GCCardTable::getDirties(std::vector<size_t>& dirtyIndices) const
{
    
}
