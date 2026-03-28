#ifndef TRAP_H
#define TRAP_H

#include "entity.hpp"

class Trap : public Entity{

    private:
        int damage;
    
    public:
        Trap(int x, int y, int trap_damage);
        int getDamage() const;

        bool isTrap() const override;
        void onSteppedOn(Entity* activator) override;

        EntityType getEntityType() const override { return EntityType::TRAP; }

        virtual void removeFromBoard(GameBoard& board) override;

        void save(std::ostream& out) const override;
        void load(std::istream& in) override;
};

#endif