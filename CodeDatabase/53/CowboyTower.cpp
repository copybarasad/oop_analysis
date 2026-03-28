#include  "CowboyTower.h"
#include "Constants.h"


using namespace GameConstants;
using namespace std;

CowboyTower::CowboyTower(int x, int y, int health, int cooldown, int range)
    : Building(x, y, health, true), attackCooldown(cooldown), attackRange(range), currentCooldown(cooldown)  {}


void CowboyTower::attack(GameField& field, Player& player) {
    int playerX = player.getX();
    int playerY = player.getY();
  
    int distX = std::abs(getX() - playerX);
    int distY = std::abs(getY() - playerY);

    if (distX <= attackRange && distY <= attackRange) {
        cout << "Ковбойская башня атакует игрока и наносит " << TOWER_ATTACK_DAMAGE << " урона!" << std::endl;
        player.take_damage(TOWER_ATTACK_DAMAGE);
    }
}

void CowboyTower::update(GameField& field, Player& player) {
    if (currentCooldown > 0) {
        currentCooldown--;
    } else {
        currentCooldown = attackCooldown;
        attack(field, player);
    }
}


int CowboyTower::getAttackCooldown() const { return attackCooldown; }
int CowboyTower::getAttackRange() const { return attackRange; }
int CowboyTower::getCurrentCooldown() const { return currentCooldown;}

void CowboyTower::setAttackCooldown(int cooldown) { attackCooldown = cooldown; }
void CowboyTower::setAttackRange(int range) { attackRange = range; }
void CowboyTower::setCurrentCooldown(int cooldown) {currentCooldown = cooldown;}
