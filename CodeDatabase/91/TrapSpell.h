#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include "ISpell.h"

class TrapSpell : public ISpell {
    private:
        int damage;
        int range;
        std::string name;
        std::string description;
        int manaCost;

    public:
        TrapSpell(int spellDamage = 40, int spellRange = 2,
                  const std::string& splellName = "Bear Trap", int cost = 20);

        bool cast(Game& game, const Position& target) const override;
        bool canTarget(const Game& game, const Position& casterPos, const Position& target) const override;

        // === Getters ===
        std::string getName() const override;
        std::string getDescription() const override;
        int getManaCost() const override;
        int getRange() const override;
        int getDamage() const;
        char getSymbol() const override;

        void upgrade() override;
};

#endif