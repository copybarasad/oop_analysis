#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include "spell.hpp"
#include "defines.hpp"

class TrapSpell : public Spell{

    public:
        bool cast(GameBoard& board, Player& player, int targetX, int targetY) override;
        std::string getName() const override;
        std::string getDescription() const override;
        double getRange() const override;

        int getTypeId() const override { return SPELL_ID_TRAP; }
};

#endif