#include "Player.h"

Player::Player(int hp, int meleeDmg, int rangedDmg)
    : health(hp), maxHealth(hp),
      meleeDamage(meleeDmg), rangedDamage(rangedDmg),
      score(0), mode(CombatMode::MELEE),
      canMove(true), slowed(false), position{-1, -1} {}

int Player::getHealth() const { return health; }
int Player::getMaxHealth() const { return maxHealth; }
int Player::getScore() const { return score; }
CombatMode Player::getMode() const { return mode; }

int Player::getDamage() const {
    return (mode == CombatMode::MELEE) ? meleeDamage : rangedDamage;
}

bool Player::canPlayerMove() const {
    return canMove && !slowed;
}

bool Player::isPlayerSlowed() const {
    return slowed;
}

Pos Player::getPosition() const {
    return position;
}

void Player::takeDamage(int dmg) {
    health -= dmg;
    if (health < 0) health = 0;
}

void Player::heal(int amount) {
    health += amount;
    if (health > maxHealth) health = maxHealth;
}

void Player::addScore(int points) { score += points; }

void Player::switchMode() {
    mode = (mode == CombatMode::MELEE) ? CombatMode::RANGED : CombatMode::MELEE;
}

void Player::setMoveAllowed(bool allowed) { canMove = allowed; }
void Player::setSlowed(bool isSlowed) { slowed = isSlowed; }

void Player::resetMovement() {

    if (slowed) {
        slowed = false;
        canMove = false;
    } else {
        canMove = true;
    }
}

void Player::setPosition(int x, int y) {
    position.x = x;
    position.y = y;
}

void Player::setPosition(const Pos& pos) {
    position = pos;
}

void Player::increaseMaxHealth(int amount) {
    maxHealth += amount;
    health += amount;
}

void Player::increaseDamage(int amount) {
    meleeDamage += amount;
    rangedDamage += amount;
}

void Player::restoreFullHealth() {
    health = maxHealth;
}

void Player::setMaxHealth(int hp) {
    maxHealth = hp;
}

void Player::setHealth(int hp) {
    health = hp;
    if (health > maxHealth) health = maxHealth;
    if (health < 0) health = 0;
}

void Player::setDamage(int dmg) {
    meleeDamage = dmg;
    rangedDamage = dmg;
}

bool Player::isAlive() const { return health > 0; }