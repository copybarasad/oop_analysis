#pragma once
#include "Spell.h"

class DirectDamageSpell : public Spell {
private:
    int damage;
    int range;
    int level = 1;

public:
    DirectDamageSpell(int damage = 15, int range = 5);
    ~DirectDamageSpell() override = default;

    // Использовать заклинание
    bool use(Player& player, GameState& state, int targetX, int targetY) override;

    // Использовать заклинание на игрока (нужен для башни)
    bool useOnPlayer(Player& player, GameState& state);

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