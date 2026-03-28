#include "turn_processor.h"
#include "player.h"
#include "enemy_manager.h"
#include "building_manager.h"
#include "combat_manager.h"
#include "tower.h"
#include <iostream>
#include <stdexcept>

TurnProcessor::TurnProcessor(Player& player, EnemyManager& enemyManager,
                           BuildingManager& buildingManager, CombatManager& combatManager,
                           bool& gameRunning, std::vector<Tower>& towers)
    : player(player), enemyManager(enemyManager), buildingManager(buildingManager),
      combatManager(combatManager), gameRunning(gameRunning), towers(towers) {}

void TurnProcessor::processGameTurn() {
    try {
        //0. Игрок атакует врагов в зоне досягаемости
        combatManager.processPlayerAttack(player, enemyManager.getEnemies());

        // 1. Обрабатываем ход зданий (спавн врагов)
        buildingManager.processBuildingTurns();
        
        // 2. Спавним врагов из зданий
        buildingManager.spawnEnemiesFromBuildings(enemyManager, player);
        
        // 3. Обрабатываем ход врагов (движение и атака)
        enemyManager.processEnemyTurns(player, combatManager);
        
        // 4. Обрабатываем атаки башен
        combatManager.processTowerAttacks(towers, player);
        
        // 5. Обрабатываем эффекты ловушек
        combatManager.processTrapEffects(enemyManager.getEnemies());
        
        // 6. Удаляем мертвых врагов и начисляем очки игроку
        enemyManager.removeDeadEnemies(player);
        
        // 7. Проверяем условие завершения игры (смерть игрока)
        if (!player.isAlive()) {
            gameRunning = false;
            std::cout << "Player has been defeated! Game over." << std::endl;
        }
        
        // 8. Восстанавливаем ману игрока в начале каждого хода
        player.restoreMana(5);
        
        // 9. Проверяем возможность движения игрока
        player.setCanMove(true);
        
    } catch (const std::exception& e) {
        std::cerr << "Error in game turn processing: " << e.what() << std::endl;
        throw std::runtime_error("Turn processing failed: " + std::string(e.what()));
    }
}

void TurnProcessor::processCombatModeSwitch() {
    try {
        // При смене режима боя игрок автоматически атакует врагов в зоне досягаемости
        combatManager.processPlayerAttack(player, enemyManager.getEnemies());
        
        // Восстанавливаем немного маны при успешной смене режима
        player.restoreMana(2);
        
        std::cout << "Combat mode switched to: " 
                  << (player.getCombatMode() == CombatMode::MELEE ? "MELEE" : "RANGED") 
                  << std::endl;
                  
    } catch (const std::exception& e) {
        std::cerr << "Error during combat mode switch: " << e.what() << std::endl;
        throw std::runtime_error("Combat mode switch failed: " + std::string(e.what()));
    }
}