#ifndef DAMAGE_SPELL_H
#define DAMAGE_SPELL_H

#include "ISpell.h"

class DamageSpell : public ISpell {
    private:
        int damage;
        int range;
        std::string name;
        std::string description;
        int manaCost;

        public:
            DamageSpell(int spellDamage = 30, int spellRange = 3,
                        const std::string& spellName = "Fire Bolt",
                        int cost = 15);

            bool cast(Game& game, const Position& target) const override;
            bool canTarget(const Game& game, const Position& casterPos, const Position& target) const;

            // === Getters ===
            std::string getName() const override;
            std::string getDescription() const override;
            int getManaCost() const override;
            int getRange() const override;
            int getDamage() const;
            char getSymbol() const override;

            // === Setters ===
            void setDamage(int d);

            void upgrade() override;
};

#endif