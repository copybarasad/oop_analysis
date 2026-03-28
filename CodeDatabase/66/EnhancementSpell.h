#ifndef ENHANCEMENTSPELL_H
#define ENHANCEMENTSPELL_H

#include "Spell.h"

class EnhancementSpell : public Spell {
public:
    EnhancementSpell();
    
    bool canCast(const Position& playerPos, const SpellTarget& target, const GameWorld& world) const override;
    void cast(const Position& playerPos, const SpellTarget& target, GameWorld& world, EnhancementBuffer& buffer) override;
    SpellType getType() const override;
    std::string getName() const override;
    std::string getDescription(const EnhancementBuffer& buffer) const override;
};

#endif
