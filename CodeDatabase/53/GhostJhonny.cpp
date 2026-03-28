#include "GhostJhonny.h"
#include "GameField.h"
#include "Player.h"
#include "Teamate.h"
#include "Constants.h"
#include <iostream>

using namespace GameConstants;

GhostJhonny::GhostJhonny(int level) 
    : SpellBase("GhostJohnny", level, DAMAGE_GHOSTJOHNNY) {
    
    if (level > BEGIN_LEVEL_GHOSTJOHNNY && level <= MAX_SPELL_LEVEL) {
        maxCntTeamates = MAX_CNT_GHOSTJOHNNY_PER_LEVEL * level;
    } else {
        maxCntTeamates = BEGIN_MAX_CNT_GHOSTJOHNNY;
    }
    
    curCntTeamates = 0;
    teamateName = "JhonyGhost";
    teamateHealth = HEALTH_JOHNNY;
    teamateDamage = DAMAGE_GHOSTJOHNNY;
    rangeSpawn = DEFAULT_RANGE_SPAWN;
}

bool GhostJhonny::use(int x, int y, GameField& field, Player& player) {
    int spawnX = x;
    int spawnY = y;

    if (canSpawn(spawnX, spawnY, player, field)) {
        if(curCntTeamates < maxCntTeamates) {
            Teamate* teamate = new Teamate(teamateName, teamateHealth, teamateDamage, spawnX, spawnY);
            field.arr_teamates.push_back(std::unique_ptr<Teamate>(teamate));
            field.setCellType(spawnX, spawnY, JHONNYGHOST);
            curCntTeamates++;
            std::cout << "Союзник успешно призван на позицию (" << spawnX << ", " << spawnY << ")!" << std::endl;
            return true;
        } else {
            std::cout << "Количество допустимых призывов JhonyGhost достигло максимума: (" 
                      << curCntTeamates << "/" << maxCntTeamates << ")" << std::endl;
        }
    }
    return false;
}


bool GhostJhonny::canSpawn(int spawnX, int spawnY, const Player& player, GameField& field) {
    int playerX = player.getX();
    int playerY = player.getY();
    
    if(field.canMoveTo(spawnX, spawnY)) {
        if(field.isValidPosition(spawnX, spawnY)) {
            int distX = std::abs(playerX - spawnX);
            int distY = std::abs(playerY - spawnY);
            
            if(distX <= rangeSpawn && distY <= rangeSpawn) {
                return true;
            } else {
                std::cout << "Точка (x, y) выходит за допустимую зону призыва" << std::endl;
                return false;
            }
        } else {
            std::cout << "Точка (x, y) выходит за пределы карты" << std::endl;
            return false;
        }
    }
    std::cout << "Эта клетка занята. JhonnyGhost не может призваться" << std::endl;
    return false;
}

int GhostJhonny::getMaxSpawns() const {
    return maxCntTeamates;
}

int GhostJhonny::getCurrentSpawns() const {
    return curCntTeamates;
}


void GhostJhonny::upgrade() {
    if (canUpgrade()) {
        setLevel(getLevel() + 1);
        maxCntTeamates = MAX_CNT_GHOSTJOHNNY_PER_LEVEL * getLevel();
    }
}


bool GhostJhonny::canUpgrade() {
    return getLevel() < MAX_SPELL_LEVEL;
}

void GhostJhonny::resetLevel() {
    setLevel(BEGIN_LEVEL_GHOSTJOHNNY);
    maxCntTeamates = BEGIN_MAX_CNT_GHOSTJOHNNY;
}

void GhostJhonny::setCurrentSpawns(int curSpawn){
    curCntTeamates = curSpawn;
}