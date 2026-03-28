#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "spell.hpp"
#include "defines.hpp"

class DirectDamageSpell : public Spell{

    public:
        bool cast(GameBoard& board, Player& player, int targetX, int targetY) override;
        std::string getName() const override;
        std::string getDescription() const override;
        double getRange() const override;
        
        int getTypeId() const override { return SPELL_ID_DIRECT_DAMAGE; }
};

#endif