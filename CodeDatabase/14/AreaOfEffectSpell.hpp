#ifndef AREA_OF_EFFECT_SPELL_H
#define AREA_OF_EFFECT_SPELL_H

#include "spell.hpp"
#include "defines.hpp"

class AreaOfEffectSpell : public Spell{

    public:
        bool cast(GameBoard& board, Player& player, int targetX, int targetY) override;
        std::string getName() const override;
        std::string getDescription() const override;
        double getRange() const override;

        int getTypeId() const override { return SPELL_ID_AOE; }
};

#endif