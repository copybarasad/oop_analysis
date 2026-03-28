#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "entity.hpp"
#include "position.hpp"
#include <cstdlib>

// простой враг, наследник сущности
class Enemy : public Entity {
public:
    Enemy(int hp, int dmg, Position p): Entity(hp, dmg, p) {}

    // выбор шага к цели по манхэттену (на 1 клетку)
    static Position chooseStep(const Position& from, const Position& to);
};
#endif