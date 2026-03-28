#include "../../headers/objects/EnemyTower.h"
#include "../../headers/objects/Player.h"
#include <sstream>
#include <iostream>

using namespace std;

EnemyTower::EnemyTower(int x, int y, int lvl) : position(x, y), level(lvl) {
    health = 100 + (lvl - 1) * 20;
    damage = 5 + (lvl - 1) * 1;
    attackRange = 2 + (lvl - 1);
    canAttack = true;
}

Position EnemyTower::getPosition() const { return position; }

bool EnemyTower::isAlive() const { 
    return health > 0; 
}

void EnemyTower::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

void EnemyTower::attackIfPossible(const Position& playerPos, shared_ptr<Player> player) {
    if (!canAttack || !isAlive()) return;
    
    double distance = position.distanceTo(playerPos);
    if (distance <= attackRange) {
        player->takeDamage(damage);
        cout << "Tower attacked you for " << damage << " damage!\n";
        canAttack = false;
    }
}

void EnemyTower::resetAttack() {
    canAttack = true;
}

int EnemyTower::getHealth() const { return health; }

int EnemyTower::getLevel() const { return level; }

string EnemyTower::serialize() const {
    return "Tower " + position.toString() + " " + to_string(health) + " " + 
           to_string(damage) + " " + to_string(attackRange) + " " + 
           to_string(level);
}

shared_ptr<EnemyTower> EnemyTower::deserialize(const string& data) {
    stringstream ss(data);
    string type;
    ss >> type;
    if (type != "Tower") throw invalid_argument("Not a tower");

    string posStr;
    int health, damage, range, level;
    if (!(ss >> posStr >> health >> damage >> range >> level)){
        throw invalid_argument("Invalid tower data format");
    }

    Position pos = Position::fromString(posStr);
    auto tower = make_shared<EnemyTower>(pos.getX(), pos.getY(), level);
    tower->health = health;
    tower->damage = damage;
    tower->attackRange = range;
    tower->level = level;
    return tower;
}