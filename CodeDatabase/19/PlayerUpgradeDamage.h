#pragma once

#include <cmath>
#include <string>
#include <sstream>

#include "core/GameLogic.h"
#include "upgrade/PlayerUpgrade.h"

constexpr int DAMAGE_BONUS = 1;

class PlayerUpgradeDamage : public PlayerUpgrade {
public:
    std::string text(int level) override {
        return "Damage: +" + std::to_string(DAMAGE_BONUS);
    }

    int price(int level) override {
        return level * 10 + 50;
    }

    std::string id() override {
        return "damage_upgrade";
    }

    void apply(const GameLogic &logic, int level) override {
        logic.player().add_attribute(Attribute::Damage, DAMAGE_BONUS);
    }
};