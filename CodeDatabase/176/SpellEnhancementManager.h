#ifndef SPELL_ENHANCEMENT_MANAGER_H
#define SPELL_ENHANCEMENT_MANAGER_H

#include "Spell.h"
#include <memory>

class SpellEnhancementManager {
private:
    int enhancementStacks;
    
public:
    SpellEnhancementManager();
    
    void addEnhancementStack();
    std::unique_ptr<Spell> applyEnhancement(std::unique_ptr<Spell> spell);
    void resetEnhancement();
    int getEnhancementStacks() const;
    bool hasEnhancement() const;
};

#endif
