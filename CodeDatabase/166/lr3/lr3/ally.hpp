#ifndef ALLY_HPP_INCLUDED
#define ALLY_HPP_INCLUDED

#include "entity.hpp"
#include "position.hpp"
#include "enemy.hpp"
#include <vector>
#include <cmath>

// Союзник теперь тоже наследуется от Entity.
// Внешний интерфейс (конструктор, getPos/setPos, getHp/setHp, getDmg/setDmg, isDead, takeDamage, утилиты) сохранён.
class Ally : public Entity {
public:
    Ally() = default;
    // Порядок аргументов сохраняем как был (Position, hp, dmg),
    // внутри вызываем конструктор базового класса (hp, dmg, Position).
    Ally(Position p, int hp, int dmg) : Entity(hp, dmg, p) {}

    // Совместимость со старым кодом
    const Position& getPos() const noexcept { return getPosition(); }
    void setPos(const Position& p) noexcept { Entity::setPos(p); }
    int  getHp() const noexcept { return Entity::getHp(); }
    void setHp(int v) noexcept { hp_ = v; }
    int  getDmg() const noexcept { return Entity::getDmg(); }
    void setDmg(int v) noexcept { dmg_ = v; }

    bool isDead() const noexcept { return Entity::isDead(); }
    void takeDamage(int d) { Entity::takeDamage(d); }

    static inline int manh(const Position& a, const Position& b) {
        return std::abs(a.getX() - b.getX()) + std::abs(a.getY() - b.getY());
    }
   //индекс ближайшего врага
    int closestEnemyIndex(const std::vector<Enemy>& enemies) const {
        int best = -1, bestD = 1e9;
        for (int i=0;i<(int)enemies.size();++i) {
            if (enemies[i].isDead()) continue;
            int d = manh(getPos(), enemies[i].getPosition());
            if (d < bestD) { bestD = d; best = i; }
        }
        return best;
    }

    static Position stepToward(const Position& from, const Position& to) {
        Position s{0,0};
        if (from.getX() < to.getX()) s.setX(1); else if (from.getX() > to.getX()) s.setX(-1);
        if (from.getY() < to.getY()) s.setY(1); else if (from.getY() > to.getY()) s.setY(-1);
        if (s.getX() && s.getY()) s.setY(0);
        return s;
    }
};

#endif
