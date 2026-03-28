#ifndef PLAYER_PROGRESSION_H
#define PLAYER_PROGRESSION_H

#include "Player.h"
#include "Hand.h"
#include <memory>
#include <string>

class PlayerProgression {
public:
    static void applyLevelUp(Player& player, Hand& hand);
    static void showUpgradeMenu(Player& player);
    static bool upgradePlayerStat(Player& player, const std::string& statType, int cost);
    static bool upgradeSpell(Hand& hand, int spellIndex, Player& player);
    
private:
    static void removeRandomSpells(Hand& hand, int percentage);
};

#endif