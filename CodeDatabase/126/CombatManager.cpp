#include "../../headers/managers/CombatManager.h"
#include "../../headers/objects/Player.h"
#include "../../headers/objects/Enemy.h"
#include "../../headers/objects/EnemyTower.h"
#include "../../headers/gamelogic/GameState.h"
#include <chrono>
#include <random>

using namespace std;

CombatManager::CombatManager(GameState& state) : gameState(state) {}

void CombatManager::performRangedAttack(Direction dir) {
    auto player = gameState.getPlayer();
    auto& field = gameState.getField();
    Position attackPos = player->getPosition();
    bool hitEnemy = false;
    
    for (int i = 1; i <= player->getRangedRange(); i++) {
        attackPos = attackPos.getNeighbour(dir);
        
        if (!field.isValidPosition(attackPos)) {
            break;
        }
        
        if (field.isCellBlocked(attackPos)) {
            cout << "Ranged attack blocked by obstacle!\n";
            break;
        }
        
        auto enemies = gameState.getEnemies();
        for (auto& enemy : enemies) {
            if (enemy->isAlive() && enemy->getPosition() == attackPos) {
                enemy->takeDamage(player->getDamage());
                cout << "Ranged attack hit enemy! Enemy health: " << enemy->getHealth() << "\n";
                if (!enemy->isAlive()) {
                    player->addScore(20);
                    player->addEnemyDefeated();
                    cout << "Enemy defeated! Score: " << player->getScore() << "\n";
                }
                hitEnemy = true;
                break;
            }
        }
        
        auto towers = gameState.getTowers();
        for (auto& tower : towers) {
            if (tower->isAlive() && tower->getPosition() == attackPos) {
                tower->takeDamage(player->getDamage());
                cout << "Ranged attack hit tower! Tower health: " << tower->getHealth() << "\n";
                if (!tower->isAlive()) {
                    player->addScore(20);
                    player->addEnemyDefeated();
                    cout << "Tower defeated! Score: " << player->getScore() << "\n";
                }
                hitEnemy = true;
                break;
            }
        }
        
        if (hitEnemy) {
            break;
        }
    }
    
    if (!hitEnemy) {
        cout << "Ranged attack missed!\n";
    }
}

void CombatManager::processEnemyTurns(){
    auto& field = gameState.getField();
    auto player = gameState.getPlayer();
    
    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    mt19937 gen(seed);
    uniform_int_distribution<> dis_dir(0, 3);
    
    auto enemies = gameState.getEnemies();
    for(auto& enemy : enemies){
        if(!enemy->isAlive()) continue;
        
        Direction direction = static_cast<Direction>(dis_dir(gen));
        Position new_pos = enemy->getPosition().getNeighbour(direction);
        
        if(field.isValidPosition(new_pos) && !field.isCellBlocked(new_pos)){
            if(new_pos == player->getPosition()){
                player->takeDamage(enemy->getDamage());
                cout << "Enemy attacked you! Health: " << player->getHealth() << "\n";
            }
            else if(field.isCellFree(new_pos)){
                enemy->moving(new_pos);
            }
        }
    }
    
    auto towers = gameState.getTowers();
    for (auto& tower : towers) {
        tower->attackIfPossible(player->getPosition(), player);
    }
    
    for (auto& tower : towers) {
        tower->resetAttack();
    }
}

void CombatManager::movePlayer(Direction dir) {
    auto player = gameState.getPlayer();
    auto& field = gameState.getField();
    Position new_pos = player->getPosition().getNeighbour(dir);
    
    if(field.isValidPosition(new_pos)){
        if(field.isCellBlocked(new_pos)){
            cout << "Cannot move - cell is blocked!\n";
            return;
        }
        
        bool enemy_found = false;
        auto enemies = gameState.getEnemies();
        for(auto& enemy : enemies){
            if(enemy->isAlive() && enemy->getPosition() == new_pos){
                enemy->takeDamage(player->getDamage());
                cout << "You attacked enemy with " << player->getWeaponName() << "! Enemy health: " << enemy->getHealth() << "\n";
                if(!enemy->isAlive()){
                    player->addScore(20);
                    player->addEnemyDefeated();
                    cout << "Enemy defeated! Score: " << player->getScore() << "\n";
                }
                enemy_found = true;
                break;
            }
        }
        
        auto towers = gameState.getTowers();
        for (auto& tower : towers) {
            if (tower->isAlive() && tower->getPosition() == new_pos) {
                tower->takeDamage(player->getDamage());
                cout << "You attacked the tower! Tower health: " << tower->getHealth() << "\n";
                if(!tower->isAlive()){
                    player->addScore(20);
                    cout << "Enemy Tower defeated! Score: " << player->getScore() << "\n";
                }
                enemy_found = true;
                break;
            }
        }
        
        if(!enemy_found && field.isCellFree(new_pos)){
            player->moving(new_pos);
        }
    } else {
        cout << "Cannot move outside the field boundaries!\n";
    }
    
    gameState.updateFieldState();
}