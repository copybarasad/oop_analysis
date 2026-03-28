#include "../include/EnhancementBuffer.h"

EnhancementBuffer::EnhancementBuffer() {
    enhancementCounts[SpellType::DIRECT_DAMAGE] = 0;
    enhancementCounts[SpellType::AREA_DAMAGE] = 0;
    enhancementCounts[SpellType::TRAP] = 0;
    enhancementCounts[SpellType::SUMMON] = 0;
}

void EnhancementBuffer::addEnhancement() {
    enhancementCounts[SpellType::DIRECT_DAMAGE]++;
    enhancementCounts[SpellType::AREA_DAMAGE]++;
    enhancementCounts[SpellType::TRAP]++;
    enhancementCounts[SpellType::SUMMON]++;
}

int EnhancementBuffer::getEnhancementCount(SpellType spellType) const {
    auto it = enhancementCounts.find(spellType);
    if (it != enhancementCounts.end()) {
        return it->second;
    }
    return 0;
}

int EnhancementBuffer::getTotalEnhancementCount() const {
    int total = 0;
    for (const auto& pair : enhancementCounts) {
        total += pair.second;
    }
    return total;
}

void EnhancementBuffer::reset() {
    for (auto& pair : enhancementCounts) {
        pair.second = 0;
    }
}
