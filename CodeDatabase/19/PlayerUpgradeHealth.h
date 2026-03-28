#pragma once

#include <string>
#include "upgrade/PlayerUpgrade.h"

constexpr int HP_BONUS = 10;

class PlayerUpgradeHealth : public PlayerUpgrade {
public:
    std::string text(int level) override {
        return "Health: +" + std::to_string(HP_BONUS);
    }

    int price(int level) override {
        return level * 10 + 45;
    }

    std::string id() override {
        return "health_upgrade";
    }

    void apply(const GameLogic &logic, int level) override {
        logic.player().add_attribute(Attribute::MaxHealth, HP_BONUS);
        logic.player().add_attribute(Attribute::Health, HP_BONUS);
    }
};