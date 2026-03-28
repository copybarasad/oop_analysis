#pragma once
#include "Spell.h"
#include "../game_objects/Ally.h"

class SpawnAllySpell: public Spell {
private:
    int range;
    int allyHealth;
    int allyDamage;
    char allySymbol;
    int count;
    int level = 1;

public:
    SpawnAllySpell(int range = 1, int allyHealth = 60, int allyDamage = 10, char allySymbol = 'A');
    ~SpawnAllySpell() override = default;

    // Использовать заклинание
    bool use(Player& player, GameState& state, int targetX, int targetY) override;

    // Геттеры
    std::string getName() const override;
    char getSymbol() const override;
    int getRange() const override;
    int getLevel() const override;
    int getAllyHealth() const;
    int getAllyDamage() const;

    // Методы для улучшения
    void upgrade() override;
    void updateStats() override;
};