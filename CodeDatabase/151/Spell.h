#pragma once
#ifndef SPELL_H
#define SPELL_H

#include "Position.h"
#include <string>

class GameField;  // Forward declaration

class Spell {
protected:
    std::string name;
    int manaCost;

public:
    Spell(const std::string& name, int manaCost);
    virtual ~Spell() = default;

    // Основной метод применения заклинания
    virtual bool cast(GameField* field, const Position& casterPos, const Position& targetPos) = 0;

    // Геттеры
    std::string getName() const;
    int getManaCost() const;

    // Виртуальный метод для клонирования (для руки игрока)
    virtual Spell* clone() const = 0;
};

#endif