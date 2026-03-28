#ifndef UPGRADE_SYSTEM_H
#define UPGRADE_SYSTEM_H

#include "player.h"

class UpgradeSystem {
public:
    static void showUpgradeMenu(Player& player);
    static void removeRandomSpells(Player& player);
    
private:
    static const int HEALTH_BOOST = 3;
    static const int DAMAGE_BOOST = 1;
    static const int MANA_BOOST = 2;
};

#endif