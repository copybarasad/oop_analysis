#include "combat_manager.h"
#include <cmath>
#include <algorithm>
#include <iostream>

CombatManager::CombatManager(GameField& field) : field(field) {}

bool CombatManager::isInMeleeRange(const Position& playerPos, const Position& enemyPos) const {
    int distance = std::abs(enemyPos.getX() - playerPos.getX()) + 
                   std::abs(enemyPos.getY() - playerPos.getY());
    return distance == 1;
}

bool CombatManager::isInRangedRange(const Position& playerPos, const Position& enemyPos) const {
    int distance = std::abs(enemyPos.getX() - playerPos.getX()) + 
                   std::abs(enemyPos.getY() - playerPos.getY());
    return distance >= 2 && distance <= 3;
}

bool CombatManager::canAttack(const Position& playerPos, CombatMode playerMode, const Enemy& enemy) const {
    Position enemyPos = enemy.getPosition();
    
    if (playerMode == CombatMode::MELEE) {
        return isInMeleeRange(playerPos, enemyPos);
    } else {
        return isInRangedRange(playerPos, enemyPos);
    }
}

void CombatManager::processPlayerAttack(Player& player, std::vector<Enemy>& enemies) {
    Position playerPos = player.getPosition();
    CombatMode playerMode = player.getCombatMode();
    
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (canAttack(playerPos, playerMode, *it) && it->isAlive() && player.isAlive()) {
            it->takeDamage(player.getDamage());
            
            if (!it->isAlive()) {
                player.addScore(10);
                player.onEnemyDefeated();
                field.setCellOccupied(it->getPosition(), false);
                it = enemies.erase(it);
                continue;
            }
        }
        ++it;
    }
}

void CombatManager::processDirectDamageSpell(Player& player, DirectDamageSpell* spell, 
                                           const Position& target, std::vector<Enemy>& enemies, 
                                           std::vector<EnemyBuilding>& buildings) {
    if (!spell->canCast(player.getPosition(), target)) {
        std::cout << "❌ Cannot cast " << spell->getName() << " - target out of range!" << std::endl;
        return;
    }
    
    // Проверяем врагов
    for (auto it = enemies.begin(); it != enemies.end(); ++it) {
        if (it->getPosition() == target && it->isAlive()) {
            int oldHealth = it->getHealth();
            it->takeDamage(spell->getDamage());
            std::cout << "🔥 " << spell->getName() << " hit enemy for " << spell->getDamage() << " damage! ";
            std::cout << "(" << oldHealth << " → " << it->getHealth() << " HP)" << std::endl;
            
            if (!it->isAlive()) {
                player.addScore(10);
                player.onEnemyDefeated();
                field.setCellOccupied(target, false);
                enemies.erase(it);
                std::cout << "💀 Enemy defeated! +10 score" << std::endl;
            }
            return;
        }
    }
    
    // Проверяем здания
    for (auto& building : buildings) {
        if (building.getPosition() == target) {
            std::cout << "🏠 " << spell->getName() << " hit building!" << std::endl;
            return;
        }
    }
    
    std::cout << "❌ No valid target for " << spell->getName() << std::endl;
}

void CombatManager::processAreaDamageSpell(Player& player, AreaDamageSpell* spell, 
                                         const Position& target, std::vector<Enemy>& enemies, 
                                         std::vector<EnemyBuilding>&) {
    if (!spell->canCast(player.getPosition(), target)) {
        return;
    }
    
    int areaSize = spell->getAreaSize(); 
    int damage = spell->getDamage(); 
    
    // Наносим урон по области 2x2
    for (int dx = 0; dx < areaSize; ++dx) {
        for (int dy = 0; dy < areaSize; ++dy) {
            Position areaPos(target.getX() + dx, target.getY() + dy);
            
            if (!field.isPositionPassable(areaPos)) {
                continue;
            }
            
            for (auto& enemy : enemies) {
                if (enemy.getPosition() == areaPos && enemy.isAlive()) {
                    enemy.takeDamage(damage);
                }
            }
        }
    }

    // Удаляем убитых врагов и начисляем очки один раз за каждого
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (!it->isAlive()) {
            player.addScore(10);
            player.onEnemyDefeated();
            field.setCellOccupied(it->getPosition(), false);
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }
}

void CombatManager::processTrapSpell(Player& player, TrapSpell* spell, const Position& target) {
    if (!spell->canCast(player.getPosition(), target)) {
        return;
    }
    
    if (field.isPositionPassable(target) && !field.getCell(target).getIsOccupied()) {
        activeTraps.push_back(target);
        std::cout << "Trap placed at (" << target.getX() << ", " << target.getY() << ")" << std::endl;
    }
}

void CombatManager::processSpellCast(Player& player, int spellIndex, const Position& target,
                                   std::vector<Enemy>& enemies, std::vector<EnemyBuilding>& buildings,
                                   std::vector<Tower>& towers) {
    
    if (spellIndex < 0 || spellIndex >= player.getSpellHand().getSpellCount()) {
        return;
    }
    
    SpellCard* spell = player.getSpellHand().getSpell(spellIndex);
    if (!spell || !player.canCastSpell(spell->getManaCost())) {
        return;
    }
    
    // Тратим ману
    player.setMana(player.getMana() - spell->getManaCost());
    
    // Применяем заклинание в зависимости от типа
    if (auto* directSpell = dynamic_cast<DirectDamageSpell*>(spell)) {
        processDirectDamageSpell(player, directSpell, target, enemies, buildings);
    } else if (auto* areaSpell = dynamic_cast<AreaDamageSpell*>(spell)) {
        processAreaDamageSpell(player, areaSpell, target, enemies, buildings);
    } else if (auto* trapSpell = dynamic_cast<TrapSpell*>(spell)) {
        processTrapSpell(player, trapSpell, target);
    }
    
    // Удаляем заклинание из руки после использования
    player.getSpellHand().removeSpell(spellIndex);
    
    // Игрок тратит ход на применение заклинания
    player.setCanMove(false);

    // Убрать предупреждение о неиспользуемом параметре
    (void)towers;
}

void CombatManager::processTowerAttacks(std::vector<Tower>& towers, Player& player) {
    for (auto& tower : towers) {
        tower.update(); 
        
        if (tower.canAttack() && tower.isInRange(player.getPosition()) && player.isAlive()) {
            tower.attack(); 
            
            int oldHealth = player.getHealth();
            int towerDamage = tower.getDamage();
            player.takeDamage(towerDamage);
            
            std::cout << "🏹 Tower attacks! Damage: " << towerDamage << " ("
                      << oldHealth << " → " << player.getHealth() << " HP)" << std::endl;
            std::cout << "   Tower cooldown: " << tower.getCooldown() << " turns" << std::endl;
        }
    }
}

void CombatManager::checkTrapTrigger(const Position& position, std::vector<Enemy>& enemies, Player& player) {
    auto trapIt = std::find(activeTraps.begin(), activeTraps.end(), position);
    if (trapIt != activeTraps.end()) {
        // Находим врага на этой позиции
        for (auto it = enemies.begin(); it != enemies.end(); ++it) {
            if (it->getPosition() == position && it->isAlive()) {
                it->takeDamage(12);
                std::cout << "Trap triggered! Enemy took 12 damage." << std::endl;
                
                if (!it->isAlive()) {
                    player.addScore(10);
                    player.onEnemyDefeated();
                    field.setCellOccupied(position, false);
                    enemies.erase(it);
                    std::cout << "💀 Enemy defeated by trap! +10 score" << std::endl;
                }
                break;
            }
        }
        // Удаляем ловушку после срабатывания
        activeTraps.erase(trapIt);
    }
}

void CombatManager::processTrapEffects(std::vector<Enemy>& enemies) {
    (void)enemies;
    activeTraps.clear();
}

const std::vector<Position>& CombatManager::getActiveTraps() const {
    return activeTraps;
}