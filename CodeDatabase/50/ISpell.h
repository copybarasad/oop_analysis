#pragma once
#include <string>
#include "GameField.h"
#include "Player.h"

class ISpell {
public:
    virtual ~ISpell() = default;

    // Информация о заклинании
    virtual std::string getName() const = 0;
    virtual int getRange() const = 0;
    virtual int getManaCost() const = 0;
    virtual double getDamage() const = 0;
    virtual void setDamage(double dmg) = 0;
    // Проверка возможности применения и сам эффект
    virtual bool canCast(const Field& field, const Player& player) const = 0;
    virtual void cast(Field& field, Player& player) = 0;

    virtual ISpell* clone() const = 0;

};
