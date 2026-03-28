#ifndef SPELLCARD_H
#define SPELLCARD_H

#include <memory>
#include "SpellCastInfo.h"

class Player;
class GameField;

// Базовый класс для всех заклинаний
class SpellCard {
public:
    virtual ~SpellCard() = default;

    // Основные методы заклинания
    virtual bool canCast(const Player& caster, const GameField& field,
        const SpellCastInfo& cast_info) const = 0;
    virtual bool cast(Player& caster, GameField& field,
        const SpellCastInfo& cast_info) = 0;
    virtual std::unique_ptr<SpellCard> clone() const = 0;

    // Характеристики заклинания
    virtual int getManaCost() const = 0;
    virtual int getCastRange() const = 0;

    // Методы для системы улучшений
    virtual void increaseDamage(int amount) = 0;
    virtual void increaseRange(int amount) = 0;
    virtual int getDamage() const = 0;
    virtual bool isAreaSpell() const = 0;
};

#endif