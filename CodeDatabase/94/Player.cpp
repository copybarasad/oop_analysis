#include "Player.h"
#include <stdexcept>

// ============================================================================
// РЕАЛИЗАЦИЯ КЛАССА PLAYER (ИГРОК)
// ============================================================================

// Конструктор игрока
Player::Player(int initialHealth, int meleeDmg, int rangedDmg) 
    : Character(initialHealth, meleeDmg), 
      score(0), meleeDamage(meleeDmg), 
      rangedDamage(rangedDmg), currentCombatMode(CombatMode::MELEE),
      hand(2), position(0, 0) {
    
    // Явная инициализация здоровья игрока
    setHealth(initialHealth);
    setMaxHealth(initialHealth);
}

// Получение текущего счета игрока
int Player::getScore() const {
    return score;
}

// Получение текущего режима боя
CombatMode Player::getCombatMode() const {
    return currentCombatMode;
}

// Получение урона в зависимости от режима боя
int Player::getCurrentDamage() const {
    return (currentCombatMode == CombatMode::MELEE) ? meleeDamage : rangedDamage;
}

// Добавление очков игроку
void Player::addScore(int points) {
    // Защита от отрицательных очков
    if (points < 0) {
        throw std::invalid_argument("Score points cannot be negative");
    }
    score += points;
}

// Переключение режима боя между ближним и дальним
void Player::switchCombatMode() {
    currentCombatMode = (currentCombatMode == CombatMode::MELEE) 
                       ? CombatMode::RANGED 
                       : CombatMode::MELEE;
}

// Лечение игрока
void Player::heal(int amount) {
    // Защита от отрицательного лечения
    if (amount < 0) {
        throw std::invalid_argument("Heal amount cannot be negative");
    }
    
    Character::heal(amount);
}

// Добавление заклинания в руку игрока
bool Player::addSpell(std::unique_ptr<Spell> spell) {
    return hand.addSpell(std::move(spell));
}

// Создание копии игрока (паттерн Прототип)
Character* Player::clone() const {
    return new Player(*this);
}

// Выполнение действия игрока
void Player::performAction() {
    // В текущей реализации игрок управляется пользователем через GameController
    // Этот метод может быть использован для автоматических действий игрока
}