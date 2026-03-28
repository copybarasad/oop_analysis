#include "PlayerActionSystem.h"
#include <iostream>
#include <algorithm>

PlayerActionSystem::PlayerActionSystem(Player& player, GameField& field,
                                     std::vector<Enemy>& enemies, std::vector<Trap>& traps,
                                     std::vector<Ally>& allies, std::vector<EnemyTower>& towers,
                                     std::vector<EnemyBuilding>& buildings,
                                     MovementSystem& movementSystem, CombatSystem& combatSystem,
                                     EnemyAISystem& enemyAISystem, BuildingManager& buildingManager,
                                     AllyManager& allyManager,
                                     IGameNotifier& notifier)
    : player(player), field(field), enemies(enemies), traps(traps), allies(allies),
      towers(towers), buildings(buildings), movementSystem(movementSystem),
      combatSystem(combatSystem), enemyAISystem(enemyAISystem),
      buildingManager(buildingManager), allyManager(allyManager),
      notifier(notifier) {}

void PlayerActionSystem::processMove(const Position &direction) {
    if (!player.getCanMove()) {
        std::cout << "Player is slowed! Skipping move.\n";
        player.setCanMove(true);
        return;
    }

    Position oldPosition = player.getPosition();
    Position newPosition = movementSystem.calculateNewPosition(player.getPosition(), direction);
    
    if (movementSystem.isValidMove(newPosition)) {
        Enemy *enemyAtTarget = getEnemyAtPosition(newPosition);
        if (enemyAtTarget == nullptr) {
            movementSystem.movePlayer(player, newPosition);
            
            if (oldPosition != newPosition) {
                notifier.notifyPlayerMoved(oldPosition, newPosition);
            }
            const GameCell &cell = field.getCell(newPosition);
            if (cell.getType() == CellType::SLOWING) {
                std::cout << "Player stepped on slowing terrain! You will skip next move.\n";
                player.setCanMove(false);
            }
            for (auto it = traps.begin(); it != traps.end();) {
                if (it->getPosition() == newPosition) {
                    int trapDamage = it->getDamage();
                    std::cout << "Player stepped on a trap! took " << trapDamage << " damage.\n";
                    player.takeDamage(trapDamage);
                    notifier.notifyPlayerDamaged(trapDamage, "Trap");
                    
                    it = traps.erase(it);
                } else {
                    ++it;
                }
            }
        } else {
            std::cout << "Cannot move there - enemy occupies the cell!\n";
        }
    } else {
        std::cout << "Cannot move there - invalid position or obstacle!\n";
    }

    allyManager.updateAllies(allies, enemies, player);
    processEnemyTurns();
    updateBuildings();
    removeDeadEnemies();
    checkCollisions();
}

void PlayerActionSystem::processAreaAttack() {
    int damage = player.getDamage();
    int attackRange = player.getAttackRange();
    int enemiesHit = 0;
    std::vector<std::pair<Position, int>> killedEnemies;
    
    for (auto &enemy : enemies) {
        if (!enemy.isAlive()) continue;
        
        Position enemyPos = enemy.getPosition();
        Position playerPos = player.getPosition();
        int distance = std::abs(enemyPos.x - playerPos.x) + std::abs(enemyPos.y - playerPos.y);
        
        if (distance <= attackRange) {
            enemy.takeDamage(damage);
            player.addScore(10);
            enemiesHit++;
            
            notifier.notifyPlayerAttacked("Enemy (Area)", damage);
            
            if (!enemy.isAlive()) {
                player.addScore(20);
                player.addCoins(15);
                killedEnemies.emplace_back(enemyPos, 15);
            }
        }
    }
    
    for (auto &tower : towers) {
        if (!tower.isAlive()) continue;
        
        Position towerPos = tower.getPosition();
        Position playerPos = player.getPosition();
        int distance = std::abs(towerPos.x - playerPos.x) + std::abs(towerPos.y - playerPos.y);
        
        if (distance <= attackRange) {
            tower.takeDamage(damage);
            notifier.notifyPlayerAttacked("Tower (Area)", damage);
            
            if (!tower.isAlive()) {
                player.addScore(20);
                player.addCoins(15);
                
                notifier.notifyTowerDestroyed(towerPos);
            }
        }
    }
    
    for (const auto& [pos, coins] : killedEnemies) {
        notifier.notifyEnemyKilled(pos, coins);
    }
    
    std::cout << "Area attack hit " << enemiesHit << " enemies!\n";
    
    processEnemyTurns();
    updateBuildings();
    removeDeadEnemies();
    checkCollisions();
}

void PlayerActionSystem::processPlayerRangedAttack(const Position &direction) {
    Position playerPos = player.getPosition();
    int range = player.getAttackRange();
    bool hitEnemy = false;
    
    for (int i = 1; i <= range; i++) {
        Position checkPos(playerPos.x + direction.x * i, playerPos.y + direction.y * i);
        if (!field.isValidPosition(checkPos)) continue;
        
        Enemy *targetEnemy = getEnemyAtPosition(checkPos);
        if (targetEnemy != nullptr && targetEnemy->isAlive()) {
            int damage = player.getDamage();
            targetEnemy->takeDamage(damage);
            player.addScore(10);
            hitEnemy = true;
            notifier.notifyPlayerAttacked("Enemy (Ranged)", damage);
            
            if (!targetEnemy->isAlive()) {
                player.addScore(20);
                player.addCoins(15);
                notifier.notifyEnemyKilled(checkPos, 15);
            }
            break;
        }
        
        EnemyTower *tower = getTowerAtPosition(checkPos);
        if (tower != nullptr && tower->isAlive()) {
            int damage = player.getDamage();
            tower->takeDamage(damage);
            player.addScore(10);
            hitEnemy = true;
            notifier.notifyPlayerAttacked("Tower (Ranged)", damage);
            
            if (!tower->isAlive()) {
                player.addScore(20);
                player.addCoins(15);
                notifier.notifyTowerDestroyed(checkPos);
            }
            break;
        }
    }
    
    if (!hitEnemy) {
        std::cout << "No enemies in attack direction within range " << range << "!\n";
    }
}

void PlayerActionSystem::processAttack(const Position &direction) {
    if (direction.x == 0 && direction.y == 0) { 
        std::cout << "Invalid attack direction!\n"; 
        return; 
    }
    
    if (player.getCombatType() == CombatType::MELEE) {
        Position attackPosition = movementSystem.calculateNewPosition(player.getPosition(), direction);
        if (!field.isValidPosition(attackPosition)) { 
            std::cout << "Invalid attack direction!\n"; 
            return; 
        }
        
        Enemy *targetEnemy = getEnemyAtPosition(attackPosition);
        if (targetEnemy == nullptr) {
            EnemyTower *tower = getTowerAtPosition(attackPosition);
            if (tower != nullptr && tower->isAlive()) {
                int damage = player.getDamage();
                tower->takeDamage(damage);
                std::cout << "Player attacked tower for " << damage << " damage!\n";
                notifier.notifyPlayerAttacked("Tower", damage);
                
                if (!tower->isAlive()) {
                    std::cout << "Tower destroyed! +20 coins\n";
                    player.addScore(20);
                    player.addCoins(15);
                    notifier.notifyTowerDestroyed(attackPosition);
                }
            } else {
                std::cout << "No enemy to attack in that direction!\n";
            }
            return;
        }
        
        AttackResult result = combatSystem.performPlayerAttack(player, *targetEnemy);
        if (result == AttackResult::SUCCESS) {
            int damage = player.getDamage();
            std::cout << "Player melee attacked enemy for " << damage << " damage!\n";
            notifier.notifyPlayerAttacked("Enemy", damage);
            
            if (!targetEnemy->isAlive()) {
                std::cout << "Enemy defeated! +20 coins\n";
                player.addScore(20);
                player.addCoins(15);
                notifier.notifyEnemyKilled(attackPosition, 15);
                
                removeDeadEnemies();
            }
        } else if (result == AttackResult::OUT_OF_RANGE) {
            std::cout << "Enemy is out of melee range!\n";
        }
    } else {
        processPlayerRangedAttack(direction);
    }
    
    processEnemyTurns();
    updateBuildings();
    checkCollisions();
}

void PlayerActionSystem::processSwitchCombat() {
    player.switchCombatType();
    CombatType newType = player.getCombatType();
    std::cout << "Switched to " << (newType == CombatType::MELEE ? "Melee" : "Ranged") << " combat. ";
    std::cout << "Damage: " << player.getDamage() << ", Range: " << player.getAttackRange() << "\n";
    processEnemyTurns();
    updateBuildings();
    checkCollisions();
}

void PlayerActionSystem::processEnemyTurns() {
    enemyAISystem.updateEnemies(enemies);
    enemyAISystem.updateTowers(towers);
}

void PlayerActionSystem::updateBuildings() {
    buildingManager.updateBuildings(enemies, field);
}

void PlayerActionSystem::removeDeadEnemies() {
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), 
                  [](const Enemy &enemy){ return !enemy.isAlive(); }), enemies.end());
}

void PlayerActionSystem::checkCollisions() {
    for (const auto &enemy: enemies) {
        if (enemy.isAlive() && enemy.getPosition() == player.getPosition()) {
            std::cout << "Enemy collided with player! ";
            player.takeDamage(enemy.getDamage());
            std::cout << "Player took " << enemy.getDamage() << " damage!\n";
            break;
        }
    }
    
    TrapManager::checkTrapCollisions(traps, enemies, player);
}

Enemy* PlayerActionSystem::getEnemyAtPosition(const Position &position) {
    for (auto &enemy: enemies) {
        if (enemy.isAlive() && enemy.getPosition() == position) return &enemy;
    }
    return nullptr;
}

EnemyTower* PlayerActionSystem::getTowerAtPosition(const Position &position) {
    for (auto &tower: towers) {
        if (tower.getPosition() == position && tower.isAlive()) return &tower;
    }
    return nullptr;
}