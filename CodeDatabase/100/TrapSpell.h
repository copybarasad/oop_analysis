#pragma once
#include "Spell.h"

class TrapSpell : public Spell {
private:
    int damage;
    int range;
    int level = 1;

public:
    TrapSpell(int damage = 25, int range = 5);
    ~TrapSpell() override = default;

    // Использовать заклинание
    bool use(Player& player, GameState& state, int targetX, int targetY) override;

    // Геттеры
    std::string getName() const override;
    char getSymbol() const override;
    int getRange() const override;
    int getLevel() const override;
    int getDamage() const;

    // Методы для улучшения
    void upgrade() override;
    void updateStats() override;
};