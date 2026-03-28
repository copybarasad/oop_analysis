#include <iostream>
#include "entity.h"
#include "game_field.h"  
#include "traps.h"
#include "single_target.h"
#include "enemy_tower.h"
#include "prints.h"

Entity::Entity(int hp, int posX, int posY, std::string name, char symbol, EntityGroup group, int dmg, int xp)
    : hp(hp), posX(posX), posY(posY), name(std::move(name)), symbol(symbol), group(group), dmg(dmg), xp(xp), alive(true)
{
    {
        maxHp = hp;
    }
}

void Entity::move(int dx, int dy, GameField& field) {
    int new_x = posX + dx;
    int new_y = posY + dy;

    if ((0 <= new_x && new_x < field.getSizeX()) && (0 <= new_y && new_y < field.getSizeY())) {
        Cell& target_cell = field.getCell(new_y, new_x);

        if (target_cell.getType() == IMPASSABLE) {
            if (name == "Player") {
                Prints::print_impassable_cell_string();
            }
            return;
        }

        if (target_cell.hasEntity() && target_cell.getEntity()->getName() == "EnemyTower") {
            if (name == "Player") {
                Entity* tower = target_cell.getEntity();
                this->attack(tower);
                if (!tower->isAlive()) {
                    Prints::print_tower_destroyed_string();
                    target_cell.setEntity(nullptr);
                }
            }
            return;
        }

        if (target_cell.hasEntity()) {
            Entity* other = target_cell.getEntity();
            if (other->isAlive()) {
                
                this->attack(other);
                if (!other->isAlive()) {
                    target_cell.setEntity(nullptr);
                } else {
                    return;  
                }
            } else {
                target_cell.setEntity(nullptr);
            }
        }

        if (target_cell.getType() == TRAPS && group == EVIL) {

            Traps trap;
            if(this->isAlive()){
                this->takeDamage(trap.getDamage());
                Prints::print_enemy_trapped_string();
                if (!this->isAlive()) {
                    Prints::print_enemy_died_from_trap_string();
                    target_cell.setEntity(nullptr);
                }
                target_cell = Cell(PASSABLE);
            }
        }

        field.getCell(posY, posX).setEntity(nullptr);

        posX = new_x;
        posY = new_y;
        
        field.getCell(posY, posX).setEntity(this);

    } else {
        if (name == "Player") {
            Prints::print_out_of_range_string();
        }
    }
}

int Entity::getPosX() const {
    return posX;
}

int Entity::getPosY() const {
    return posY;
}

char Entity::getSym() const {
    return symbol;
}

int Entity::getHp() const {
    return hp;
}
int Entity::getMaxHp() const{
    return maxHp;
};

void Entity::increaseMaxHp(int amount) {
    maxHp += amount;
    setHp(maxHp);
}

int Entity::getXp() const{
    return xp;
}

EntityGroup Entity::getGroup() const
{
    return group;
}

bool Entity::isAlive() const {
    return alive;
}

void Entity::setPos(int x, int y){
    this->posX = x;
    this->posY = y;
}

void Entity::setHp(int hp){
    this->hp = hp;
}

void Entity::setXp(int xp){
    this->xp = xp;
}

void Entity::setAlive(bool alive){
    this->alive = alive;
}

void Entity::takeDamage(int dmg) {
    hp -= dmg;
    if (hp <= 0) {
        alive = false;
    }
}

void Entity::attack(Entity* other) {
    other->takeDamage(dmg);
    if (!other->isAlive() && name == "Player") {
        xp += 5;
        Prints::print_entity_killed_other(name, other->getName());
    }
}

GameField* Entity::currentField = nullptr;

const std::string& Entity::getName() const {
    return name;
}

void Entity::setEntityOnField(GameField& field) {
    field.getCell(posY, posX).setEntity(this);
}