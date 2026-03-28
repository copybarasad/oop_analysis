#include "player.h"

Player::Player(int id, int health, int meleeDamage, int rangedDamage, int score)
    : Entity(id, health),
      meleeDamage_(meleeDamage),
      rangedDamage_(rangedDamage),
      meleeRange_(1),
      rangedRange_(3),
      isRanged_(false),
      isSlowed_(false),
      score_(score),
      hand_(5) {}

EntityType Player::getType() const { return EntityType::Player; }

int Player::getDamage() const {
    return isRanged_ ? rangedDamage_ : meleeDamage_;
}

int Player::getAttackRange() const {
    return isRanged_ ? rangedRange_ : meleeRange_;
}

void Player::toggleCombatMode() {
    isRanged_ = !isRanged_;
}

void Player::applySlow() {
    isSlowed_ = true;
}

bool Player::canMove() const {
    return !isSlowed_;
}

void Player::resetSlow() {
    isSlowed_ = false;
}

void Player::addScore(int amount) {
    score_ += amount;
    if (score_ < 0) score_ = 0;
    if (score_ > 9999) score_ = 9999;
}

int Player::getScore() const {
    return score_;
}

bool Player::isRanged() const {
    return isRanged_;
}

Player* Player::asPlayer() { return this; }

Hand& Player::hand() {
    return hand_;
}

void Player::storeBuffForNextSpell(int extraRange, int extraArea, int extraTrapDamage, int extraSummons) {
    extraRange_ = extraRange;
    extraArea_ = extraArea;
    extraTrapDamage_ = extraTrapDamage;
    extraSummons_ = extraSummons;
}

int Player::consumeExtraRange() {
    int v = extraRange_;
    extraRange_ = 0;
    return v;
}

int Player::consumeExtraArea() {
    int v = extraArea_;
    extraArea_ = 0;
    return v;
}

int Player::consumeExtraTrapDamage() {
    int v = extraTrapDamage_;
    extraTrapDamage_ = 0;
    return v;
}

int Player::consumeExtraSummons() {
    int v = extraSummons_;
    extraSummons_ = 0;
    return v;
}

int Player::getMeleeDamage() const {
    return meleeDamage_;
}

int Player::getRangedDamage() const {
    return rangedDamage_;
}

void Player::setMeleeDamage(int damage) {
    if (damage > 0) meleeDamage_ = damage;
}

void Player::setRangedDamage(int damage) {
    if (damage > 0) rangedDamage_ = damage;
}
