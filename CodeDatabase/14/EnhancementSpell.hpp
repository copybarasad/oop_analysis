#ifndef ENHANCEMENT_SPELL_H
#define ENHANCEMENT_SPELL_H

#include "spell.hpp"
#include "defines.hpp"

class EnhancementSpell : public Spell{

    bool cast(GameBoard& board, Player& player, int targetX, int targetY) override;
    std::string getName() const override;
    std::string getDescription() const override;
    double getRange() const override;
    bool needsTarget() const override { 
        return false; 
    }

    int getTypeId() const override { return SPELL_ID_ENHANCE; }
};

#endif