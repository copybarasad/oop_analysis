#include "BuildingCombat.h"
#include <algorithm>

BuildingCombat::BuildingCombat(Wanderer& wanderer, BuildingManager& buildingManager, 
                               GameField& field, const Position& wandererPos)
    : wanderer(wanderer), buildingManager(buildingManager), 
      field(field), wandererPosition(wandererPos) {}

void BuildingCombat::processMeleeBuildingAttack(const Position& targetPos) {
    auto& buildings = buildingManager.getBuildings();
    auto& towers = buildingManager.getTowers();
    
    for (auto& buildingData : buildings) {
        if (buildingData.first.x == targetPos.x && buildingData.first.y == targetPos.y) {
            int damage = wanderer.getCurrentDamage();
            buildingData.second.takeDamage(damage);
            
            std::cout << "Melee attack! You hit beast building for " << damage << " damage! ";
            std::cout << "Building health: " << buildingData.second.getHealth() << "/" << buildingData.second.getMaxHealth() << "\n";
            
            if (buildingData.second.isDestroyed()) {
                destroyBuildingReward(targetPos, false);
            }
            return;
        }
    }
    
    for (auto& towerData : towers) {
        if (towerData.first.x == targetPos.x && towerData.first.y == targetPos.y) {
            int damage = wanderer.getCurrentDamage();
            towerData.second.takeDamage(damage);
            
            std::cout << "Melee attack! You hit enemy tower for " << damage << " damage! ";
            std::cout << "Tower health: " << towerData.second.getHealth() << "/" << towerData.second.getMaxHealth() << "\n";
            
            if (towerData.second.isDestroyed()) {
                destroyBuildingReward(targetPos, true);
            }
            return;
        }
    }
    
    std::cout << "No building to attack at this position!\n";
}

void BuildingCombat::processRangedBuildingAttack(int directionX, int directionY) {
    bool attacked = false;
    
    auto& buildings = buildingManager.getBuildings();
    auto& towers = buildingManager.getTowers();
    
    auto attackBuilding = [&](const Position& pos, bool isTower) {
        int vecX = pos.x - wandererPosition.x;
        int vecY = pos.y - wandererPosition.y;
        
        int distance = std::max(std::abs(vecX), std::abs(vecY));
        
        if (distance >= 2 && distance <= 3) {
            if ((directionX == 0 || (vecX * directionX > 0)) &&
                (directionY == 0 || (vecY * directionY > 0))) {

                if (!field.hasLineOfSight(wandererPosition.x, wandererPosition.y, pos.x, pos.y)) {
                    return false;
                }
                
                int damage = wanderer.getCurrentDamage();
                
                if (isTower) {
                    for (auto& towerData : towers) {
                        if (towerData.first.x == pos.x && towerData.first.y == pos.y) {
                            towerData.second.takeDamage(damage);
                            std::cout << "Ranged attack! You shoot enemy tower for " << damage << " damage! ";
                            std::cout << "Tower health: " << towerData.second.getHealth() << "/" << towerData.second.getMaxHealth() << "\n";
                            
                            if (towerData.second.isDestroyed()) {
                                destroyBuildingReward(pos, true);
                            }
                            return true;
                        }
                    }
                } else {
                    for (auto& buildingData : buildings) {
                        if (buildingData.first.x == pos.x && buildingData.first.y == pos.y) {
                            buildingData.second.takeDamage(damage);
                            std::cout << "Ranged attack! You shoot beast building for " << damage << " damage! ";
                            std::cout << "Building health: " << buildingData.second.getHealth() << "/" << buildingData.second.getMaxHealth() << "\n";
                            
                            if (buildingData.second.isDestroyed()) {
                                destroyBuildingReward(pos, false);
                            }
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    };
    
    for (auto& towerData : towers) {
        if (attackBuilding(towerData.first, true)) {
            attacked = true;
            break;
        }
    }
    
    if (!attacked) {
        for (auto& buildingData : buildings) {
            if (attackBuilding(buildingData.first, false)) {
                attacked = true;
                break;
            }
        }
    }
    
    if (!attacked) {
        std::cout << "Ranged: no buildings in that direction (2-3 cells away)\n";
    }
}

void BuildingCombat::destroyBuildingReward(const Position& pos, bool isTower) {
    int points = isTower ? 100 : 75;
    int exp = isTower ? 80 : 60;
    
    wanderer.addPoints(points);
    wanderer.addExp(exp);
    
    std::cout << (isTower ? "Enemy tower" : "Beast building") << " destroyed! ";
    std::cout << "+" << points << " points! +" << exp << " experience!\n";
    
    field.getCell(pos.x, pos.y).setHasBuilding(false);
}

void BuildingCombat::updateWandererPosition(const Position& newPos) {
    wandererPosition = newPos;
}