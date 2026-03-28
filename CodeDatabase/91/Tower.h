#ifndef TOWER_H
#define TOWER_H

#include "ITower.h"
#include "../Spells/DamageSpell.h"

class Tower : public ITower {
    private:
        Position position_;
        int range_;
        int cooldown_;
        int currentCooldown_;
        bool active_;
        DamageSpell attack_spell_;
        int health_;

    public:
        explicit Tower(const Position& pos, int towerRange = 3, int health = 100);

        void update(Game& game) override;
        bool canAttack(const Game& game) const override;
        void attack(Game& game) override;

        void takeDamage(int damage) override;
        bool isAlive() const;
        int getHealth() const;

        Position getPosition() const override;
        int getRange() const override;
        bool isActive() const override;
        int getCooldown() const override;
        char getSymbol()  const override;

        void applyLevelBonus(int level) override;

        void setActive(bool isActtive);

        // === JSON ===
        const DamageSpell& getAttackSpell() const;

        void loadState(const nlohmann::json& j);
};

#endif