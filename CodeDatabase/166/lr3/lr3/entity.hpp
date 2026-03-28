#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "position.hpp"
#include <stdexcept>

// базовый класс для любого существа на поле(герой/враг)
class Entity {
public:
    // проверяем инварианты в конструкторе
    Entity(int hp, int dmg, Position pos) : hp_(hp), dmg_(dmg), pos_(pos) {
        if (hp <= 0) throw std::invalid_argument("HP must be positive");
        if (dmg <= 0) throw std::invalid_argument("Damage must be positive");
        if (pos.getX() < 0 || pos.getY() < 0) throw std::invalid_argument("Position coordinates must be non-negative");
    }

    virtual ~Entity() = default;

    // геттеры
    int getHp() const noexcept { return hp_; }
    int getDmg() const noexcept { return dmg_; }
    const Position& getPosition() const noexcept { return pos_; }

    // изменение позиции
    void setPos(Position pos) noexcept { pos_ = pos; }

    // получаем урон
    void takeDamage(int dmg) noexcept { hp_ -= dmg; if (hp_ < 0) hp_ = 0; }

    // умерли?
    bool isDead() const noexcept { return hp_ <= 0; }

protected:
    // характеристики сущности
    int hp_;
    int dmg_;
    Position pos_;
};
#endif

