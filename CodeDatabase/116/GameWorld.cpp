#include "GameWorld.h"
#include "../spells/AreaDamageSpell.h"
#include "../spells/DirectFightSpell.h"
#include "../spells/TrapSpell.h"
#include "../spells/SummonSpell.h"
#include "../spells/EnhancingSpell.h"

GameWorld::GameWorld(int width, int height)
    : field(width, height)
    , player(std::make_unique<Player>(width/2, height/2, 100, 15, 25))
{}

GameWorld::~GameWorld() = default;

void GameWorld::clearWorld() {
    enemies.clear();
    allies.clear();
    traps.clear();
    fortresses.clear();
}

const GameField& GameWorld::getField() const{
    return field;
}

GameField& GameWorld::getField(){
    return field;
}

void GameWorld::setTower(int x, int y, int Health, int frequency) {
    tower = std::make_unique<Building>(x, y, Health, frequency);
}

const std::vector<std::unique_ptr<Enemy>>& GameWorld::getEnemies() const {
    return enemies;
}

std::vector<std::unique_ptr<Enemy>>& GameWorld::getEnemies() {
    return enemies;
}

const std::vector<std::unique_ptr<Ally>>& GameWorld::getAllies() const {
    return allies;
}

std::vector<std::unique_ptr<Ally>>& GameWorld::getAllies() {
    return allies;
}

const std::vector<std::unique_ptr<Trap>>& GameWorld::getTraps() const {
    return traps;
}

std::vector<std::unique_ptr<Trap>>& GameWorld::getTraps() {
    return traps;
}

const std::vector<std::unique_ptr<Fortress>>& GameWorld::getFortresses() const {
    return fortresses;
}

std::vector<std::unique_ptr<Fortress>>& GameWorld::getFortresses() {
    return fortresses;
}

int GameWorld::getEnemiesCount() const{
    return enemies.size();
}

void GameWorld::moveCharacter(int oldX, int oldY, int newX, int newY, Character& person){
    Cell::CellType buf = field.getCell(oldX,oldY).getType();
    person.setPosition(newX,newY);
    field.getCell(oldX,oldY).setType(Cell::CellType::EMPTY);
    if(buf == Cell::CellType::PLAYER){
        field.getCell(newX,newY).setType(Cell::CellType::PLAYER);
    }
    else if (buf == Cell::CellType::ENEMY){
        field.getCell(newX,newY).setType(Cell::CellType::ENEMY);
    }
    else{
        field.getCell(newX,newY).setType(Cell::CellType::ALLY);
    }
}

const Player& GameWorld::getPlayer() const{
    return *player;
}

Player& GameWorld::getPlayer(){
    return *player;
}

Character* GameWorld::getVillain(std::pair<int, int> target){
    auto type = field.getCell(target.first,target.second).getType();
    if(type == Cell::CellType::ENEMY || type == Cell::CellType::BUILDING || type == Cell::CellType::FORTRESS){
        switch (type)
        {
        case Cell::CellType::ENEMY:{
            for(auto& en : enemies){
                if(en->getX() == target.first && en->getY() == target.second){
                    return en.get();
                }
            }
            break;
        }  
        case Cell::CellType::BUILDING:
            return tower.get();
            break;
        case Cell::CellType::FORTRESS:{
            for(auto& fr : fortresses){
                if(fr->getX() == target.first && fr->getY() == target.second){
                    return fr.get();
                }
            }
            break;
        }
        default:{
            break;
        }
        }   
        return nullptr;
    }
    return nullptr;
}

Character* GameWorld::getHero(std::pair<int, int> target){
    auto type = field.getCell(target.first,target.second).getType();
    if(type == Cell::CellType::PLAYER || type == Cell::CellType::ALLY){
        switch (type)
        {
        case Cell::CellType::ALLY:{
            for(auto& al : allies){
                if(al->getX() == target.first && al->getY() == target.second){
                    return al.get();
                }
            }
            break;
        }
        case Cell::CellType::PLAYER:{
            return player.get();
            break;
        }
        default:{
            break;
        }
        }
    }
    return nullptr;
}