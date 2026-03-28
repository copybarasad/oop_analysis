#ifndef ALLY_H
#define ALLY_H

#include "entity.hpp"
#include "defines.hpp"

class Ally : public Entity{

    private:
        int health;
        int damage;

    public:
        Ally(int x, int y, int start_healt = ALLY_HEALTH, int ally_damage = ALLY_DAMAGE);

        int getHealth() const override;
        int getDamage()  const;

        void takeDamage(int dmg) override;
        bool isAlive() const;

        EntityType getEntityType() const override { return EntityType::ALLY; }

        virtual void removeFromBoard(GameBoard& board) override;

        void save(std::ostream& out) const override;
        void load(std::istream& in) override;
};

#endif