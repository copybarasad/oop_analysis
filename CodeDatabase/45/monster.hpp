#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "entity.hpp"
#include "map.hpp"

#include <string>

class Monster : public Entity {
public:
    Monster(int health, int damage);
    void step(Map* map, int x, int y) override;
    virtual std::string getStatus() const override;
};

#endif