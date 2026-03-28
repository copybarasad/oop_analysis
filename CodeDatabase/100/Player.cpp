#include "Player.h"
#include "../interface/ConsoleRenderer.h"
#include "../game_main/GameState.h"
#include <ostream>
#include <sstream>

// Конструктор
Player::Player(int health, int damage, int startX, int startY) 
    : Character(health, damage, startX, startY, 'P'),
    score(0), combatMode(CombatMode::Melee),
    meleeDamage(damage), rangedDamage(damage / 2), isSlowed(false) { }

// Геттеры
int Player::getScore() const { return score; }

CombatMode Player::getCombatMode() const { return combatMode; }

int Player::getCurrentDamage() const { return (combatMode == CombatMode::Melee) ? meleeDamage : rangedDamage; }

// Действия
void Player::switchCombatMode(GameState& state) {
    combatMode = (combatMode == CombatMode::Melee) ? CombatMode::Ranged : CombatMode::Melee;

    std::ostringstream ss;
    ss << Colors::PINK << "Combat mode switched on " 
       << (combatMode == CombatMode::Melee ? "Melee" : "Ranged")
       << " | Damage: " << getCurrentDamage() << Colors::RESET;
    
    state.addMessage(ss.str());
}

void Player::addScore(int points) { score += points; }

void Player::setScore(int newScore) { score = newScore; }

void Player::setCombatMode(int newMode) { 
    combatMode = (newMode == 1) ? CombatMode::Ranged : CombatMode::Melee;
}

// Передвижение
void Player::move(int deltaX, int deltaY, const GameField& field) {
    if (isSlowed) {
        isSlowed = false;
        return;
    }

    Character::move(deltaX, deltaY, field);

    // Проверяем замедляющую клетку
    if (field.isCellSlowing(posX, posY)) {
        isSlowed = true;
    }

}
