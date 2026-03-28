#include "Enemy.h"
#include "Player.h"
#include "Character.h"
#include "GameField.h"
#include "Saloon.h"
#include "Constants.h"
#include <iostream>


using namespace GameConstants;

Enemy::Enemy(std::string enemyName, int startHealth, int startDamage, int startX, int startY)
    : Character(enemyName, startHealth, startDamage, startX, startY) {}

void Enemy::move(int playerX, int playerY, Player& player, GameField& field) {

    int newX = position.x;
    int newY = position.y;
    
    int diffX = std::abs(playerX - position.x);
    int diffY = std::abs(playerY - position.y);
    
    if (diffX > diffY) {
        if (position.x < playerX) newX = position.x + 1;
        else if (position.x > playerX) newX = position.x - 1;
    } else {
        if (position.y < playerY) newY = position.y + 1;
        else if (position.y > playerY) newY = position.y - 1;
    }
    
    if (newX == playerX && newY == playerY) {
        std::cout << name << " атакует игрока и наносит " << damage << " урона!" << std::endl;
        player.take_damage(damage);
    }else if(field.getCell(newX,newY).getType() == JHONNYGHOST){
        std::cout << name << " атакует JHONNYGHOST и наносит " << damage << " урона!" << std::endl;
        field.findTeamate(newX, newY).take_damage(damage);
    } else {
        if(freeze_time == 0){
            if (field.canMoveTo(newX,newY)){
                if (inSaloon){
                    field.setCellType(position.x, position.y, SALOON);
                    inSaloon = false;
                    std::cout << "Враг " << name << " покидает SALOON" << std::endl;
                } else {
                    field.setCellType(position.x, position.y, EMPTY);
                }

                position.x = newX;
                position.y = newY;

                if(field.getCell(newX, newY).getType() == SALOON) {
                    Saloon& saloon = field.findSaloon(newX, newY);
                    freeze_time = saloon.getTime_freeze();
                    inSaloon = true;
                    heal(HEAL_IN_SALOON);
                    std::cout << "Враг " << name << " вошел в SALOON! Он пропускает " << freeze_time << " ход" << std::endl;
                }

                if (field.getCell(newX, newY).getType() == TRAP) {
                    field.activateTrap(newX, newY, *this,player);
                    if(!isAlive()){
                        std::cout << "Враг " << getName() << " побежден!" << std::endl;
                        player.takeAwards(ENEMY);
                    }
                }
                
                field.setCellType(newX, newY,ENEMY);
            }
        } else{
            freeze_time--;
        }
    }
}

void Enemy::displayInfo() const {
    std::cout << "=== ИНФОРМАЦИЯ О ВРАГЕ ===" << std::endl;
    std::cout << "Имя: " << name << std::endl;
    std::cout << "Здоровье: " << health << "/" << max_health << std::endl;
    std::cout << "Урон: " << damage << std::endl;
    std::cout << "Позиция: (" << position.x << ", " << position.y << ")" << std::endl;
    std::cout << "==========================" << std::endl;
}
