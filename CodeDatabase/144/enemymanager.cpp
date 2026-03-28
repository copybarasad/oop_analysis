#include "enemymanager.h"
#include "field.h"
#include "player.h"
#include "buildingmanager.h"
#include "enemy_movement_context.h"
#include "game_logging.h"
#include <iostream>
#include <random>

void EnemyManager::spawnInitialEnemies(Field& field, BuildingManager& buildingManager, Player& player, int count,
                                       int enemyHealth, int enemyDamage, GameEventBus* eventBus) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(1, field.getWidth() - 2);
    std::uniform_int_distribution<> disY(1, field.getHeight() - 2);
    
    int playerX = player.getX();
    int playerY = player.getY();
    
    for (int i = 0; i < count; ++i) {
        int x, y;
        int attempts = 0;
        do {
            x = disX(gen);
            y = disY(gen);
            attempts++;
            if (attempts > 50) {
                std::cout << "Warning: Difficulty finding spawn position for enemy " << i << "\n";
                break;
            }
        } while (!field.canPlaceEntity(x, y, playerX, playerY) || buildingManager.isCellOccupiedByBuilding(x, y));
        
        enemies.push_back(Enemy(enemyHealth, enemyDamage, x, y));
    }
    
	if (eventBus) {
		eventBus->publish({GameEventType::System, "Spawned " + std::to_string(enemies.size()) + " enemies"});
	}
}

void EnemyManager::moveEnemies(Field& field, Player& player, const EnemyMovementContext& context, GameEventBus* eventBus) {
    int playerX = player.getX();
    int playerY = player.getY();
    
    int movedEnemies = 0;
    
    for (auto& enemy : enemies) {
        if (!enemy.isAlive()) continue;
        
        int currentX = enemy.getX();
        int currentY = enemy.getY();
        int newX = currentX;
        int newY = currentY;
        
        field.findPathToPlayer(currentX, currentY, playerX, playerY, newX, newY);
        
        if (newX == playerX && newY == playerY) {
            player.takeDamage(enemy.getDamage());
			if (eventBus) {
				eventBus->publish({GameEventType::Damage, "Player took " + std::to_string(enemy.getDamage()) + " damage from enemy"});
			}
        } 
        else if (isCellOccupiedByEnemy(newX, newY) || 
                 context.getBuildingManager().isCellOccupiedByBuilding(newX, newY) ||
                 context.isCellOccupiedByAlly(newX, newY)) {
        }
        else if (field.isCellPassable(newX, newY)) {
            enemy.setPosition(newX, newY);
            movedEnemies++;
        }
    }
    
    if (movedEnemies > 0 && eventBus) {
		eventBus->publish({GameEventType::Movement, std::to_string(movedEnemies) + " enemies advanced"});
    }
}

bool EnemyManager::attackEnemyAtPosition(int x, int y, Player& player, GameEventBus* eventBus) {
    for (auto& enemy : enemies) {
        if (enemy.isAlive() && enemy.getX() == x && enemy.getY() == y) {
            enemy.takeDamage(player.getDamage());
			if (eventBus) {
				eventBus->publish({GameEventType::Damage, "Enemy at (" + std::to_string(x) + ", " + std::to_string(y) + ") took " + std::to_string(player.getDamage()) + " damage"});
			}
            
            if (!enemy.isAlive()) {
                player.addScore(10);
				if (eventBus) {
					eventBus->publish({GameEventType::Damage, "Enemy at (" + std::to_string(x) + ", " + std::to_string(y) + ") defeated"});
				}
            }
            return true;
        }
    }
    return false;
}

void EnemyManager::performRangedAttack(Field& field, Player& player, int directionX, int directionY, GameEventBus* eventBus) {
    int playerX = player.getX();
    int playerY = player.getY();
    int range = player.getRangedAttackRange();
    
    int targetX = playerX;
    int targetY = playerY;
    
    for (int i = 0; i < range; ++i) {
        targetX += directionX;
        targetY += directionY;
        
        if (!field.isValidPosition(targetX, targetY)) break;
        
        for (auto& enemy : enemies) {
            if (enemy.isAlive() && enemy.getX() == targetX && enemy.getY() == targetY) {
                enemy.takeDamage(player.getDamage());
				if (eventBus) {
					eventBus->publish({GameEventType::Damage, "Ranged hit at (" + std::to_string(targetX) + ", " + std::to_string(targetY) + ") for " + std::to_string(player.getDamage())});
				}
                
                if (!enemy.isAlive()) {
                    player.addScore(10);
					if (eventBus) {
						eventBus->publish({GameEventType::Damage, "Enemy at (" + std::to_string(targetX) + ", " + std::to_string(targetY) + ") defeated"});
					}
                }
                return;
            }
        }
        
        if (!field.isCellPassable(targetX, targetY)) {
            std::cout << "Your attack hit a wall at (" << targetX << ", " << targetY << ")\n";
            break;
        }
    }
    
    std::cout << "Ranged attack: no enemy hit within range.\n";
}

bool EnemyManager::isCellOccupiedByEnemy(int x, int y) const {
    for (const auto& enemy : enemies) {
        if (enemy.isAlive() && enemy.getX() == x && enemy.getY() == y) {
            return true;
        }
    }
    return false;
}