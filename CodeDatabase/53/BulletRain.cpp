#include "BulletRain.h"
#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include "Barracks.h"
#include "CowboyTower.h"
#include "Constants.h"
#include <iostream>

using namespace GameConstants;

BulletRain::BulletRain(int spell_level)
    : SpellBase("BulletRain", spell_level, BULLETRAIN_DAMAGE) {
        
    if(spell_level > BEGIN_LEVEL_BULLETRAIN && spell_level <= MAX_SPELL_LEVEL) {
        areaOfEffect = AREA_OF_EFFECT_PER_LEVEL * spell_level;
    } else { 
        setLevel(BEGIN_LEVEL_BULLETRAIN); 
        areaOfEffect = BEGIN_BULLETRAIN_AREA_OF_EFFECT;
    }
}

bool BulletRain::use(int x, int y, GameField& field, Player& player) {
    if(!field.isValidPosition(x, y)) {
        std::cout << "Координаты (x,y) выходят за границы поля" << std::endl;
        return false;
    }
    
    if(!IsInAreaPosition(x, y, player)) {
        std::cout << "Координаты (x,y) вне зоны действия" << std::endl;
        return false;
    }

    Type_cell cell = field.getCell(x, y).getType();

    if(cell == ENEMY || cell == COWBOYTOWER || cell == BARRACK) {
        std::cout << "Используется заклинание BulletRain на позиции (" << x << ", " << y << ")." << std::endl;
        if(damageObject(cell, field, x, y, player)) {
            return true;
        }
        return false;
    } else {
        std::cout << "В этих координатах нет живых существ" << std::endl;
        std::cout << "Вот кого вы можете атаковать:" << std::endl;
        if(findCharactersInArea(player.getX(), player.getY(), field) == 0) {
            std::cout << "Никого нет в области действия заклинания!" << std::endl;
        }
        return false;
    }
}

bool BulletRain::IsInAreaPosition(int x, int y, Player& player) {
    int playerX = player.getX();
    int playerY = player.getY();
    
    if (x >= playerX - areaOfEffect && x <= playerX + areaOfEffect &&
        y >= playerY - areaOfEffect && y <= playerY + areaOfEffect) {
        return true;
    } else {
        std::cout << "Цель вне области действия заклинания!" << std::endl;
        return false;
    }
}

int BulletRain::findCharactersInArea(int x, int y, GameField& field) {
    int count = 0;
    
    for(int dx = x - areaOfEffect; dx <= x + areaOfEffect; dx++) {
        for(int dy = y - areaOfEffect; dy <= y + areaOfEffect; dy++) {
            int targetX = dx;
            int targetY = dy;
            
            if (field.isValidPosition(targetX, targetY) && !(targetX == x && targetY == y)) {
                Type_cell typeCell = field.getCell(targetX, targetY).getType();
                
                if (typeCell == ENEMY) {
                    Enemy& enemy = field.findEnemy(targetX, targetY);
                    count++;
                    std::cout << "BulletRain нашел " << enemy.getName() << " на позиции (" << targetX << ", " << targetY << ")." << std::endl;
                } else if (typeCell == COWBOYTOWER) {
                    count++;
                    std::cout << "BulletRain нашел CowboyTower на позиции (" << targetX << ", " << targetY << ")." << std::endl;
                }else if(typeCell == BARRACK){
                    count++;
                    std::cout << "BulletRain нашел Barrack на позиции (" << targetX << ", " << targetY << ")." << std::endl;
                }
            }
        }
    }
    
    return count;
}

bool BulletRain::damageObject(Type_cell cell, GameField& field, int x, int y, Player& player) {
    switch(cell) {
        case ENEMY: {
            Enemy& enemy = field.findEnemy(x, y);
            player.damageEnemy(enemy, getDamage());
            return true;
            break;
        }
        case BARRACK: {
            Barracks& barrack = field.findBarracks(x, y);
            player.damageBaracks(barrack, getDamage());
            return true;
            break;
        }
        case COWBOYTOWER: {
            CowboyTower& tower = field.findCowboyTower(x, y);
            player.damageCowboyTower(tower, getDamage());
            return true;
            break;
        }
        default:
            std::cout << "BulletRain не нашел цели в указанных координатах!" << std::endl;
            break;
    }
    return false;
}

void BulletRain::upgrade() {
    if (canUpgrade()) {
        setLevel(getLevel() + 1);
        areaOfEffect = AREA_OF_EFFECT_PER_LEVEL * getLevel();
    }
}

bool BulletRain::canUpgrade()  {
    return getLevel() < MAX_SPELL_LEVEL;
}

void BulletRain::resetLevel() {
    setLevel(BEGIN_LEVEL_BULLETRAIN);
    areaOfEffect = BEGIN_BULLETRAIN_AREA_OF_EFFECT;
}

int BulletRain::getAreaOfEffect() const {
    return areaOfEffect;
}

void BulletRain::setAreaOfEffect(int area) {
    areaOfEffect = area;
}