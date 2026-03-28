#include "Player.h"

Player::Player(int hp, int meleeDamage, int rangedDamage, int rangedRange)
    : Unit(0, 0, hp, meleeDamage),
    meleeDamage_(meleeDamage),
    rangedDamage_(rangedDamage),
    rangedRange_(rangedRange),
    score_(0),
    mode_(AttackMode::Melee),
    pendingEnhancement_(),
    maxHP_(hp)
{
}

int Player::getMeleeDamage() const { return meleeDamage_; }
int Player::getRangedDamage() const { return rangedDamage_; }
int Player::getRangedRange() const { return rangedRange_; }
int Player::getScore() const { return score_; }

void Player::addScore(int s) { score_ += s; }

bool Player::spendScore(int s) {
    if (s <= 0) return true;
    if (score_ < s) return false;
    score_ -= s;
    return true;
}

Player::AttackMode Player::getAttackMode() const { return mode_; }
void Player::setAttackMode(AttackMode mode) { mode_ = mode; }

int Player::getAttackDamage() const {
    return (mode_ == AttackMode::Melee) ? meleeDamage_ : rangedDamage_;
}

void Player::addPendingEnhancement(const SpellEnhancement& enh) {
    pendingEnhancement_.extraDamage += enh.extraDamage;
    pendingEnhancement_.extraRange += enh.extraRange;
    pendingEnhancement_.extraArea += enh.extraArea;
    pendingEnhancement_.extraSummons += enh.extraSummons;
}

SpellEnhancement Player::getPendingEnhancement() const {
    return pendingEnhancement_;
}

void Player::clearPendingEnhancement() {
    pendingEnhancement_ = SpellEnhancement();
}

int Player::getMaxHP() const {
    return maxHP_;
}

void Player::restoreToFullHP() {
    hp_ = maxHP_;
}

// --- Методы апгрейда ---
void Player::addHP(int amount) {
    if (amount <= 0) return;
    maxHP_ += amount;
    hp_ += amount;
}

void Player::increaseMeleeDamage(int amount) {
    meleeDamage_ += amount;
    if (meleeDamage_ < 0) meleeDamage_ = 0;
}

void Player::increaseRangedDamage(int amount) {
    rangedDamage_ += amount;
    if (rangedDamage_ < 0) rangedDamage_ = 0;
}

void Player::increaseRangedRange(int amount) {
    rangedRange_ += amount;
    if (rangedRange_ < 0) rangedRange_ = 0;
}
