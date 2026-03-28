#include "DirectDamageSpell.h"
#include "GameState.h"
#include "Enemy.h"
#include "MovementValidator.h"
#include <iostream>

DirectDamageSpell::DirectDamageSpell(std::string spellName, int spellDamage, int spellManaCost, int spellRange) 
    : name(std::move(spellName)), damage(spellDamage), manaCost(spellManaCost), range(spellRange) {}

bool DirectDamageSpell::cast(const Position& target, GameState& gameState, const Player& caster) {
    if (!canCast(caster.getPosition(), target, gameState)) {
        std::cout << "Цель вне радиуса действия заклинания " << name << "!" << std::endl;
        return false;
    }
    
    auto& enemies = gameState.getEnemies();
    bool hitEnemy = false;
    
    for (auto& enemy : enemies) {
        if (enemy.isAlive() && enemy.getPosition() == target) {
            enemy.takeDamage(damage);
            std::cout << name << " наносит " << damage << " урона врагу!" << std::endl;
            hitEnemy = true;
            break;
        }
    }
    
    if (!hitEnemy) {
        std::cout << "Заклинание " << name << " не попало по цели! На выбранной клетке нет врага." << std::endl;
        return false;
    }
    
    return true;
}