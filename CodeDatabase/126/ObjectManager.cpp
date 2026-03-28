#include "../../headers/managers/ObjectManager.h"
#include "../../headers/objects/Player.h"
#include "../../headers/objects/Trap.h"
#include "../../headers/objects/Enemy.h"
#include "../../headers/objects/EnemyTower.h"
#include "../../headers/gamelogic/GameState.h"
#include <iostream>

using namespace std;

ObjectManager::ObjectManager(GameState& state) : gameState(state) {}

void ObjectManager::cleanupDeadObjects() {
    auto& gameObjects = gameState.getGameObjects();
    auto it = gameObjects.begin();
    while (it != gameObjects.end()) {
        if (!(*it)->isAlive()) {
            it = gameObjects.erase(it);
        } else {
            ++it;
        }
    }
}

void ObjectManager::checkTraps() {
    auto& gameObjects = gameState.getGameObjects();
    auto& field = gameState.getField();
    auto player = gameState.getPlayer();
    
    for (auto it = gameObjects.begin(); it != gameObjects.end(); ) {
        if (auto trap = dynamic_pointer_cast<Trap>(*it)) {
            bool shouldRemoveTrap = false;
            
            auto enemies = gameState.getEnemies();
            for (auto& enemy : enemies) {
                if (enemy->isAlive() && enemy->getPosition() == trap->getPosition()) {
                    cout << "Trap triggered by enemy\n";
                    enemy->takeDamage(20);
                    shouldRemoveTrap = true;
                    break;
                }
            }
            
            if (!shouldRemoveTrap) {
                auto towers = gameState.getTowers();
                for (auto& tower : towers) {
                    if (tower->isAlive() && tower->getPosition() == trap->getPosition()) {
                        cout << "Trap triggered by tower\n";
                        tower->takeDamage(20);
                        shouldRemoveTrap = true;
                        break;
                    }
                }
            }
            
            if (!shouldRemoveTrap && player->getPosition() == trap->getPosition()) {
                player->takeDamage(20);
                shouldRemoveTrap = true;
                cout << "Trap triggered by player\n";
            }
            
            if (shouldRemoveTrap) {
                cout << "Removing trap from game\n";
                field.removeTrapPosition(trap->getPosition());
                it = gameObjects.erase(it);
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }
}