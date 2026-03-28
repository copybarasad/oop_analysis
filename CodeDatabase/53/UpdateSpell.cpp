#include "UpdateSpell.h"
#include "HandSpell.h"
#include "BulletRain.h"
#include "MegaShotGun.h"
#include "CheapWhiskey.h"
#include "GhostJhonny.h"
#include "Player.h"
#include "Constants.h"
#include <iostream>


using namespace GameConstants;
using namespace std;

UpdateSpell::UpdateSpell(int curLevel, int maxLevel) 
    : SpellBase("UpdateSpell", curLevel, 0), maxLevel(maxLevel) {}

bool UpdateSpell::use(int x, int y, GameField& field, Player& player) {
    if(level < maxLevel) {
        level++;
        
        player.getHandSpell()->getBulletRain()->upgrade();
        player.getHandSpell()->getMegaShotgun()->upgrade();
        player.getHandSpell()->getCheapWhiskey()->upgrade();
        player.getHandSpell()->getGhostJhonny()->upgrade();      
        
        std::cout << "Все заклинания улучшены до уровня " << level << "!" << std::endl;
        return true;
    } else {
        std::cout << "Достигнут максимальный уровень улучшения" << std::endl;
        return false;
    }
}

void UpdateSpell::resetUpdate(HandSpell& handSpell) {
    level = BEGIN_SPELL_LEVEL;

    handSpell.getBulletRain()->resetLevel();
    handSpell.getMegaShotgun()->resetLevel();
    handSpell.getCheapWhiskey()->resetLevel();
    handSpell.getGhostJhonny()->resetLevel();

    std::cout << "Все способности сбросились до (" << level << ") уровня" << std::endl;
}

int UpdateSpell::getMaxLevel() const { 
    return maxLevel; 
}

void UpdateSpell::setMaxLevel(int newMaxLevel) { 
    maxLevel = newMaxLevel; 
}