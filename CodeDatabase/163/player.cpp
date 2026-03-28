#include "player.hpp"
#include "map.hpp"
#include "game_state.hpp"
#include <iostream>
#include <cmath>

Player::Player(int startX, int startY, Cell& cell, int hp, int range, 
               int swordDmg, int bowDmg, int handSize)
    : GameObject(startX, startY, cell, hp, range), 
      swordDamage(swordDmg), bowDamage(bowDmg), 
      currentWeapon(WeaponType::SWORD), isSlowed(false), 
      hasMovedThisTurn(false), hasAttackedThisTurn(false),
      hasCastSpellThisTurn(false),
      spellHand(std::make_unique<PlayerHand>(handSize)) {
    std::cout << "DEBUG: Создан игрок в (" << startX << "," << startY 
              << ") с HP=" << hp << "\n";
}

void Player::switchWeapon() {
    currentWeapon = (currentWeapon == WeaponType::SWORD) ? 
                    WeaponType::BOW : WeaponType::SWORD;
}

WeaponType Player::getCurrentWeapon() const {
    return currentWeapon;
}

bool Player::tryMove(int targetX, int targetY, Map& map) {
    if (isSlowed || hasMovedThisTurn) {
        return false;
    }
    
    if (targetX == positionX && targetY == positionY) {
        return false;
    }
    
    int distance = std::abs(targetX - positionX) + std::abs(targetY - positionY);
    if (distance != 1) {
        return false;
    }
    
    if (moveTo(targetX, targetY, map)) {
        hasMovedThisTurn = true;
        return true;
    }
    return false;
}

void Player::startTurn() {
    Cell& current = getCurrentCell();
    if (current.getCellType() == Cell::Type::SLOWING) {
        isSlowed = true;
    }
    resetTurnFlags();
}

void Player::endTurn() {
    isSlowed = false;
}

bool Player::getIsSlowed() const {
    return isSlowed;
}

int Player::getAttackDamage() const {
    return (currentWeapon == WeaponType::SWORD) ? swordDamage : bowDamage;
}

bool Player::canAttack() const {
    return !hasAttackedThisTurn;
}

void Player::performAttack(GameObject& target) {
    if (!canAttack()) {
        throw std::runtime_error("Player has already attacked this turn");
    }
    target.takeDamage(getAttackDamage());
    hasAttackedThisTurn = true;
}

void Player::resetAttackFlag() {
    hasAttackedThisTurn = false;
}

PlayerHand* Player::getSpellHand() {
    return spellHand.get();
}

const PlayerHand* Player::getSpellHand() const {
    return spellHand.get();
}

bool Player::canCastSpell() const {
    return !hasCastSpellThisTurn;
}

void Player::castSpell(int spellIndex, GameState* gameState, int targetX, int targetY) {
    if (!canCastSpell() || !spellHand) {
        return;
    }
    
    const SpellCard* spell = spellHand->getSpell(spellIndex);
    if (spell && gameState) {
        int dirX = 0, dirY = 0;
        if (targetX != positionX) dirX = (targetX > positionX) ? 1 : -1;
        if (targetY != positionY) dirY = (targetY > positionY) ? 1 : -1;
        
        if ((dirX == 0 && dirY == 0)) {
            std::cout << "Нельзя кастовать заклинание на себя.\n";
            return;
        }
        
        if (dirX != 0 && dirY != 0) {
            std::cout << "Заклинание должно быть направлено в одну сторону (W/A/S/D).\n";
            return;
        }
        
        spell->castAsProjectile(gameState, dirX, dirY);
        hasCastSpellThisTurn = true;
        spellHand->removeSpell(spellIndex);
    }
}

void Player::resetSpellFlag() {
    hasCastSpellThisTurn = false;
}

bool Player::canAttackInDirection(int dirX, int dirY, const Map& map) const {
    if (!isValidDirection(dirX, dirY)) {
        return false;
    }
    
    int targetX = positionX + dirX;
    int targetY = positionY + dirY;
    return map.isValidPosition(targetX, targetY);
}

bool Player::isValidDirection(int dirX, int dirY) const {
    return (dirX == 0 && dirY != 0) || (dirX != 0 && dirY == 0);
}

int Player::performDirectionalAttack(int dirX, int dirY, const Map& map,
                                    const std::vector<std::unique_ptr<GameObject>>& enemies,
                                    const std::vector<std::unique_ptr<GameObject>>& buildings) {
    if (!canAttackInDirection(dirX, dirY, map) || !canAttack()) {
        return 0;
    }
    
    int damage = getAttackDamage();
    
    if (currentWeapon == WeaponType::SWORD) {
        int targetX = positionX + dirX;
        int targetY = positionY + dirY;
        
        for (const auto& enemy : enemies) {
            if (enemy && enemy->isAlive() && 
                enemy->getPositionX() == targetX && 
                enemy->getPositionY() == targetY) {
                enemy->takeDamage(damage);
                hasAttackedThisTurn = true;
                return damage;
            }
        }
        
        for (const auto& building : buildings) {
            if (building && building->isAlive() && 
                building->getPositionX() == targetX && 
                building->getPositionY() == targetY) {
                building->takeDamage(damage);
                hasAttackedThisTurn = true;
                return damage;
            }
        }
        
        return 0;
    } else {
        int currentX = positionX + dirX;
        int currentY = positionY + dirY;
        
        for (int distance = 1; distance <= BOW_RANGE; distance++) {
            if (!map.isValidPosition(currentX, currentY)) {
                break;
            }
            
            const Cell& cell = map.getCell(currentX, currentY);
            
            for (const auto& enemy : enemies) {
                if (enemy && enemy->isAlive() && 
                    enemy->getPositionX() == currentX && 
                    enemy->getPositionY() == currentY) {
                    enemy->takeDamage(damage);
                    hasAttackedThisTurn = true;
                    return damage;
                }
            }
            
            for (const auto& building : buildings) {
                if (building && building->isAlive() && 
                    building->getPositionX() == currentX && 
                    building->getPositionY() == currentY) {
                    building->takeDamage(damage);
                    hasAttackedThisTurn = true;
                    return damage;
                }
            }
            
            if (!cell.isPassable() && cell.getCellType() != Cell::Type::SLOWING) {
                break;
            }
            
            currentX += dirX;
            currentY += dirY;
        }
        return 0;
    }
}

bool Player::canMove() const {
    return !hasMovedThisTurn && !isSlowed;
}

void Player::resetTurnFlags() {
    hasMovedThisTurn = false;
    hasAttackedThisTurn = false;
    hasCastSpellThisTurn = false;
}

void Player::takeTurn(Map& map) {
    (void)map;
}

void Player::enemyDefeated() {
    if (spellHand) {
        spellHand->enemyDefeated();
    }
}