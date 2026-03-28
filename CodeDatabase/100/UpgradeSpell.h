#pragma once
#include "Spell.h"

class UpgradeSpell : public Spell {
private:
    int range;
    const int UPGRADE_POWER = 2;

public:
    UpgradeSpell(int range = 3);
    ~UpgradeSpell() override = default;

    // Использовать заклинание
    bool use(Player& player, GameState& state, int targetX, int targetY) override;
    
    // Геттеры
    std::string getName() const override;
    char getSymbol() const override;
    int getRange() const override;
    int getLevel() const override;

    // Методы для улучшения
    void upgrade() override;
    void updateStats() override;
};