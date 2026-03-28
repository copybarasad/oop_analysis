#include "MegaShotGun.h"
#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include "Barracks.h"
#include "CowboyTower.h"
#include "Constants.h"
#include <iostream>

using namespace GameConstants;

MegaShotGun::MegaShotGun(int spell_level)
    : SpellBase("MegaShotGun", spell_level, MEGASHOTGUN_DAMAGE) {
        
    if(spell_level > BEGIN_LEVEL_MEGASHOTGUN && spell_level <= MAX_SPELL_LEVEL) {
        sizeZoneAttack = SIZE_ZONE_ATTACK_PER_LEVEL * spell_level;
    } else { 
        setLevel(BEGIN_LEVEL_MEGASHOTGUN); 
        sizeZoneAttack = BEGIN_MEGASHOTGUN_SIZE_ZONE_ATTACK;
    }
}

bool MegaShotGun::use(int x, int y, GameField& field, Player& player) {
    int playerX = player.getX();
    int playerY = player.getY();
    int targetsHit = 0;
    
    if (playerX + sizeZoneAttack == x && playerY - sizeZoneAttack == y) {
        for(int dx = playerX; dx <= x; dx++) {
            for(int dy = playerY; dy >= y; dy--) {
                if( !(dx == playerX && dy == playerY) && field.isValidPosition(dx, dy)) {
                    if (damageTheLiving(dx, dy, field, player)) {
                        targetsHit++;
                    }
                }
            }
        }
    }
    else if(playerX + sizeZoneAttack == x && playerY + sizeZoneAttack == y) {
        for(int dx = playerX; dx <= x ; dx++) {
            for(int dy = playerY; dy <= y; dy++) {
                if(!(dx == playerX && dy == playerY) && field.isValidPosition(dx, dy)) {
                    if (damageTheLiving(dx, dy, field, player)) {
                        targetsHit++;
                    }
                }
            }
        }
    }
    else if(playerX - sizeZoneAttack == x && playerY - sizeZoneAttack == y) {
        for(int dx = playerX; dx >= x; dx--) {
            for(int dy = playerY; dy >= y; dy--) {
                if(!(dx == playerX && dy == playerY) && field.isValidPosition(dx, dy)) {
                    if (damageTheLiving(dx, dy, field, player)) {
                        targetsHit++;
                    }
                }
            }
        }
    }
    else if(playerX - sizeZoneAttack == x && playerY + sizeZoneAttack == y) {
        for(int dx = playerX; dx >= x; dx--) {
            for(int dy = playerY; dy <= y; dy++) {
                if(!(dx == playerX && dy == playerY) && field.isValidPosition(dx, dy)) {
                    if (damageTheLiving(dx, dy, field, player)) {
                        targetsHit++;
                    }
                }
            }
        }
    }
    
    if(targetsHit > 0) {
        std::cout << "MegaShotgun поразил " << targetsHit << " целей!" << std::endl;
    } else {
        std::cout << "MegaShotgun никого не поразил" << std::endl;
    }
    return true;
}

bool MegaShotGun::damageTheLiving(int x, int y, GameField& field, Player& player) {
    Type_cell cellType = field.getCell(x, y).getType();
    bool targetHit = false;
    
    switch(cellType) {
        case ENEMY: {
            Enemy& enemy = field.findEnemy(x, y);
            player.damageEnemy(enemy, getDamage());
            targetHit = true;
            break;
        }
        case BARRACK: {
            Barracks& barrack = field.findBarracks(x, y);
            player.damageBaracks(barrack, getDamage());
            targetHit = true;
            break;
        }
        case COWBOYTOWER: {
            CowboyTower& tower = field.findCowboyTower(x, y);
            player.damageCowboyTower(tower, getDamage());
            targetHit = true;
            break;
        }
        default:
            break;
    }
    
    return targetHit;
}


void MegaShotGun::upgrade() {
    if (canUpgrade()) {
        setLevel(getLevel() + 1);
        sizeZoneAttack = SIZE_ZONE_ATTACK_PER_LEVEL * getLevel();
    }
}

bool MegaShotGun::canUpgrade() {
    return getLevel() < MAX_SPELL_LEVEL;
}

void MegaShotGun::resetLevel() {
    setLevel(BEGIN_LEVEL_MEGASHOTGUN);
    sizeZoneAttack = BEGIN_MEGASHOTGUN_SIZE_ZONE_ATTACK;
}

int MegaShotGun::getAreaOfEffect() const {
    return sizeZoneAttack;
}

void MegaShotGun::setAreaOfEffect(int area) {
    sizeZoneAttack = area;
}