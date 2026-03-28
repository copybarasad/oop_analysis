#pragma once


#include <string>

#include "core/GameLogic.h"
#include "upgrade/PlayerUpgrade.h"

constexpr int GOLD_BONUS = 2;

class PlayerUpgradeGold : public PlayerUpgrade {
public:
    std::string text(int level) override {
        return "Gold: +" + std::to_string(GOLD_BONUS) + " per kill";
    }

    int price(int level) override {
        return level * 50 + 50;
    }

    std::string id() override {
        return "gold_upgrade";
    }

    void apply(const GameLogic &logic, int level) override {

    }
};