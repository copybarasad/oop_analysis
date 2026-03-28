#ifndef SPELLTYPES_H
#define SPELLTYPES_H

enum class SpellType {
    DIRECT_DAMAGE,
    AREA_DAMAGE,
    TRAP,
    SUMMON,
    ENHANCE
};

struct EnhancementState {
    int rangeBonus = 0;
    int areaBonus = 0;
    int damageBonus = 0;
    int summonBonus = 0;
    int enhanceStack = 0;
    
    void reset() {
        rangeBonus = 0;
        areaBonus = 0;
        damageBonus = 0;
        summonBonus = 0;
        enhanceStack = 0;
    }
    
    bool hasEnhancements() const {
        return rangeBonus > 0 || areaBonus > 0 || damageBonus > 0 || 
               summonBonus > 0 || enhanceStack > 0;
    }
};

#endif

