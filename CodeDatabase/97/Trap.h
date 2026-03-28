#ifndef UNTITLED_TRAP_H
#define UNTITLED_TRAP_H


#include "Entity.h"
#include "Coord.h"

class Trap : public Entity {
public:
    Trap(int dmg = 9);
    ~Trap() override = default;

    std::shared_ptr<Entity> clone() const override;
    int trapDamage() const noexcept { return damage_; }
    void onTurn(Field& field, const Coord& pos) override{}
};



#endif //UNTITLED_TRAP_H
