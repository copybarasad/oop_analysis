#pragma once

#include <string>
#include "upgrade/PlayerUpgrade.h"

class PlayerUpgradeSpell : public PlayerUpgrade {
public:
    std::string text(int level) override {
        return "Spell level: +1";
    }

    int price(int level) override {
        return level * 75 + 25;
    }

    std::string id() override {
        return "spell_upgrade";
    }

    void apply(const GameLogic &logic, int level) override {
    }
};