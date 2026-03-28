#pragma once
#include "Spell.h"
#include <string>

class Player;
class GameState;

class AreaDamageSpell : public Spell {
private:
    int damage;
    int range;
    int areaSize;
    int level = 1;

public:
    AreaDamageSpell(int damage = 10, int range = 5, int areaSize = 2);
    ~AreaDamageSpell() override = default;

    // Использовать заклинание
    bool use(Player& player, GameState& state, int targetX, int targetY) override;

    // Геттеры
    std::string getName() const override;
    char getSymbol() const override;
    int getRange() const override;
    int getLevel() const override;
    int getDamage() const;
    int getAreaSize() const;

    // Методы для улучшения
    void upgrade() override;
    void updateStats() override;
};