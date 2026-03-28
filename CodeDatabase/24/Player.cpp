#include "Player.hpp"
#include <iostream>
#include <algorithm>

// Конструктор игрока
Player::Player(int startHealth, int startDamage, size_t handSize)
    : Entity(startHealth, startDamage),
      score(0),
      isRangedMode(false),
      meleeDamage(std::clamp(startDamage, Config::Constraints::MIN_DAMAGE, Config::Constraints::MAX_DAMAGE)),
      rangedDamage(std::clamp(startDamage / Config::Settings::RANGED_DAMAGE_DIVIDER,
                             Config::Constraints::MIN_DAMAGE, Config::Constraints::MAX_DAMAGE)),
    hand(handSize),
    nextSpellThreshold(Config::Settings::SPELL_AWARD_THRESHOLD) {
    // Изначально рука содержит одно случайное заклинание
    hand.addRandomSpell();
}

// Геттеры
int Player::getScore() const { return score; }
bool Player::getIsRangedMode() const { return isRangedMode; }

// Сеттеры для загрузки игры
void Player::setScore(int newScore) {
    score = newScore;
}

void Player::setRangedMode(bool ranged) {
    isRangedMode = ranged;
    setDamage(isRangedMode ? rangedDamage : meleeDamage);
}

void Player::setMeleeDamage(int damage) {
    meleeDamage = std::clamp(damage, Config::Constraints::MIN_DAMAGE, Config::Constraints::MAX_DAMAGE);
    if (!isRangedMode) {
        setDamage(meleeDamage);
    }
}

void Player::setRangedDamage(int damage) {
    rangedDamage = std::clamp(damage, Config::Constraints::MIN_DAMAGE, Config::Constraints::MAX_DAMAGE);
    if (isRangedMode) {
        setDamage(rangedDamage);
    }
}

// Добавление очков
void Player::addScore(int points) { 
    score += points;

    while (score >= nextSpellThreshold) {
        // Попытаться добавить случайную карту в руку
        // Если рука полна, ничего не делаем
        if (hand.size() < hand.capacity()) {
            hand.addRandomSpell();
        } 
        // Сдвигаем следующий порог
        nextSpellThreshold += Config::Settings::SPELL_AWARD_THRESHOLD;
    }
}

// Переключение режима боя
std::string Player::switchCombatMode() {
    isRangedMode = !isRangedMode;
    setDamage(isRangedMode ? rangedDamage : meleeDamage);
    return std::string("Combat mode switched to: ") + (isRangedMode ? "ranged" : "melee");
}

// Получение дальности атаки
int Player::getAttackRange() const {
    return isRangedMode ? Config::Settings::PLAYER_RANGED_ATTACK_RANGE 
                        : Config::Settings::PLAYER_MELEE_ATTACK_RANGE;
}

// Атака цели
void Player::attack(Entity* target) {
    if (target) {
        int dealt = getDamage();
        target->takeDamage(dealt);
        std::cout << "Player attacks and deals " << dealt << " damage. ";
        if (target->isAlive()) {
            std::cout << "Target has " << target->getHealth() << " HP remaining." << std::endl;
        } else {
            std::cout << "Target destroyed." << std::endl;
            addScore(Config::Settings::PLAYER_SCORE_PER_KILL);
        }
    }
}

// Доступ к руке
Hand& Player::getHand() { return hand; }
const Hand& Player::getHand() const { return hand; }
