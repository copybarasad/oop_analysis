#ifndef AREA_SPELL_H
#define AREA_SPELL_H

#include "ISpell.h"
#include <vector>

class AreaSpell : public ISpell {
    private:
        int damage;
        int range;
        int areaSize;
        std::string name;
        std::string description;
        int manaCost;

    public:
        AreaSpell(int spellDamage = 20, int spellRange = 2, int spellAreaSize = 2,
                  const std::string& spellName = "Fireball", int cost = 25);

        bool cast(Game& game, const Position& target) const override;
        bool canTarget(const Game& game, const Position& casterPos, const Position& target) const;

        // === Getters ===
        std::string getName() const override;
        std::string getDescription() const override;
        int getManaCost() const override;
        int getRange() const override;
        int getDamage() const;
        char getSymbol() const override;

        std::vector<Position> getAffectedArea(const Position& center) const;

        void upgrade() override;

        // === JSON ===
        int getAreaSize() const;

};

#endif