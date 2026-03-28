#include "Player.h"

Player::Player(int start_x, int start_y, int health, int melee_Damage, int ranged_Damage)
    : health(health), max_Health(health), melee_Damage(melee_Damage),
    ranged_Damage(ranged_Damage), score(0), current_Mode(CombatMode::MELEE),
    isSlowed(false), position(start_x, start_y) {
}

void Player::take_Damage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

void Player::Hill(int hill_points) {
    health += hill_points;
    if (health > max_Health) {
        health = max_Health;
    }
}

void Player::add_Score(int points) {
    score += points;
}

void Player::switch_CombatMode() {
    current_Mode = (current_Mode == CombatMode::MELEE) ? CombatMode::RANGED : CombatMode::MELEE;
}

void Player::apply_Slow() {
    isSlowed = true;
}

void Player::remove_Slow() {
    isSlowed = false;
}

bool Player::is_Alive() const {
    return health > 0;
}

int Player::get_Health() const {
    return health;
}

int Player::get_Score() const {
    return score;
}

CombatMode Player::get_CombatMode() const {
    return current_Mode;
}

int Player::get_Damage() const {
    return (current_Mode == CombatMode::MELEE) ? melee_Damage : ranged_Damage;
}

bool Player::get_IsSlowed() const {
    return isSlowed;
}

const Position& Player::get_Position() const {
    return position;
}

void Player::set_Position(const Position& newPosition) {
    position = newPosition;
}

void Player::setMaxHealth(int health) {
    max_Health = health;
    if (this->health > max_Health) {
        this->health = max_Health;
    }
}

void Player::setMeleeDamage(int damage) {
    melee_Damage = damage;
}

void Player::setRangedDamage(int damage) {
    ranged_Damage = damage;
}

void Player::increaseMaxHealth(int amount) {
    max_Health += amount;
    health += amount;
}

void Player::increaseMeleeDamage(int amount) {
    melee_Damage += amount;
}

void Player::increaseRangedDamage(int amount) {
    ranged_Damage += amount;
}

void Player::setHealth(int h) {
    health = h;
    if (health > max_Health) {
        health = max_Health;
    }
    if (health < 0) {
        health = 0;
    }
}

void Player::setScore(int s) {
    score = s;
}

void Player::setCombatMode(CombatMode mode) {
    current_Mode = mode;
}

void Player::setSlowState(bool slow) {
    isSlowed = slow;
}