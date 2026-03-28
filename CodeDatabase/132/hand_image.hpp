#pragma once

#include <string>
#include <random>
#include <algorithm>

class HandImage {
public:
    HandImage(int cap, int enh_level, std::string cards)
        : capacity(cap), enhancement_level(enh_level), cards(cards) {}

    void remove_half();

    // Z - ZapSpell
    // F - FireballSpell
    // E - SpawnEnemySpell
    // A - SummonSpell
    // T - TrapSpell
    std::string cards;

    int capacity;
    int enhancement_level;
};