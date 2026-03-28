#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

#include "entity.hpp"
#include "defines.hpp"

class EnemyTower : public Entity {

    private:
        int health;
        int damage;
        double range;
        int cooldown;
        int current_cooldown;

    public:
        EnemyTower(int x, int y);

        void takeDamage(int dmg) override;
        void onDeath(Player* player) override;
        bool isAlive() const;
        int getHealth() const override;
        int getDamage() const;
        double getRange() const;
        bool isEnemy() const override;

        bool canAttack() const;
        void tickCooldown();
        void resetCooldown();

        EntityType getEntityType() const override { return EntityType::TOWER; }

        virtual void removeFromBoard(GameBoard& board) override;

        void save(std::ostream& out) const override;
        void load(std::istream& in) override;
};

#endif