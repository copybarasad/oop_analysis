#ifndef GTOWER_HPP
#define GTOWER_HPP

#include "entity.hpp"
#include "direct_spell.hpp"
#include "map.hpp"

#include <string>

class GuardTower : public Entity {
private:
    DirectSpell spell;
    int charge;
public:
    GuardTower(int health, int damage);
    void step(Map* map, int x, int y) override;
    virtual std::string getStatus() const override;
    void setCharge(int new_charge);
};

#endif