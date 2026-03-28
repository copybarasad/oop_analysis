#include "Player.h"
#include "GameConstants.h"

Player::Player(int startX, int startY, int initialHealth, int maxX, int maxY, int handSize)
    : Character(startX, startY, initialHealth, maxX, maxY),
      meleeDamage(GameConstants::PLAYER_MELEE_DAMAGE),
      rangedDamage(GameConstants::PLAYER_RANGED_DAMAGE),
      score(GameConstants::PLAYER_SCORE_INITIAL),
      currentBattleType(BattleType::MELEE),
      skipNextTurn(false),
      hand(handSize),
      spellsCast(0) {
    if (meleeDamage <= 0 || rangedDamage <= 0) {
        throw std::invalid_argument("Damage values must be positive");
    }
}

int Player::getMeleeDamage() const {
    return meleeDamage;
}

int Player::getRangedDamage() const {
    return rangedDamage;
}

int Player::getScore() const {
    return score;
}

BattleType Player::getBattleType() const {
    return currentBattleType;
}

bool Player::shouldSkipTurn() const {
    return skipNextTurn;
}

void Player::addScore(int points) {
    score += points;
}

void Player::switchBattleType() {
    currentBattleType = (currentBattleType == BattleType::MELEE)
                            ? BattleType::RANGED
                            : BattleType::MELEE;
}

void Player::setSkipNextTurn(bool skip) {
    skipNextTurn = skip;
}

int Player::getDamage() const {
    return (currentBattleType == BattleType::MELEE) ? meleeDamage : rangedDamage;
}

Hand &Player::getHand() {
    return hand;
}

const Hand &Player::getHand() const {
    return hand;
}

bool Player::canCastSpells() const {
    return !hand.isEmpty();
}

void Player::incrementSpellsCast() {
    spellsCast++;
}

int Player::getSpellsCast() const {
    return spellsCast;
}

void Player::setHealth(int health) {
    if (health < 0) {
        throw std::invalid_argument("Health cannot be negative");
    }
    this->health = health;
}

void Player::setMeleeDamage(int damage) {
    if (damage <= 0) {
        throw std::invalid_argument("Melee damage must be positive");
    }
    meleeDamage = damage;
}

void Player::setRangedDamage(int damage) {
    if (damage <= 0) {
        throw std::invalid_argument("Ranged damage must be positive");
    }
    rangedDamage = damage;
}

void Player::setScore(int score) {
    if (score < 0) {
        throw std::invalid_argument("Score cannot be negative");
    }
    this->score = score;
}

void Player::setBattleType(BattleType type) {
    currentBattleType = type;
}
