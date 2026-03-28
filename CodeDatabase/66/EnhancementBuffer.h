#ifndef ENHANCEMENTBUFFER_H
#define ENHANCEMENTBUFFER_H

#include "SpellType.h"
#include <map>

class EnhancementBuffer {
private:
    std::map<SpellType, int> enhancementCounts;

public:
    EnhancementBuffer();
    
    void addEnhancement();
    int getEnhancementCount(SpellType spellType) const;
    int getTotalEnhancementCount() const;
    void reset();
};

#endif
