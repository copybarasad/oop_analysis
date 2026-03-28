#pragma once
#include <string>

class Player;
class GameState;

class Spell {
public:
    virtual ~Spell() = default;

    // Использовать заклинание
    virtual bool use(Player& player, GameState& state, int targetX, int targetY) = 0;

    // Геттеры
    virtual std::string getName() const = 0;
    virtual char getSymbol() const = 0;
    virtual int getRange() const = 0;
    virtual int getLevel() const = 0;

    // Методы для улучшения
    virtual void upgrade() = 0;
    virtual void updateStats() = 0;
};