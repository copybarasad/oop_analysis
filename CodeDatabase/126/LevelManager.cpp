#include "../../headers/managers/LevelManager.h"
#include "../../headers/objects/Player.h"
#include "../../headers/objects/Enemy.h"
#include "../../headers/objects/EnemyTower.h"
#include "../../headers/gamelogic/GameState.h"
#include <chrono>
#include <random>

using namespace std;

LevelManager::LevelManager(GameState& state) : gameState(state) {}

void LevelManager::startLevel() {
    gameState.setStatus(GameStatus::RUNNING);
    gameState.clearGameObjects();

    Position playerPos = gameState.getPlayer()->getPosition();
    auto& field = gameState.getField();
    if (!field.isValidPosition(playerPos) || field.isCellBlocked(playerPos)) {
        playerPos = Position(field.getWidth()/2, field.getHeight()/2);
        gameState.getPlayer()->moving(playerPos);
    }

    initializeEnemies();
    initializeTowers();
    gameState.updateFieldState();
}

void LevelManager::nextLevel() {
    int currentLevel = gameState.getCurrentLevel();
    int nextLevel = currentLevel + 1;
    gameState.setCurrentLevel(nextLevel);
    
    int newWidth = 15 + (nextLevel - 1) * 2;
    int newHeight = 15 + (nextLevel - 1) * 2;

    auto savedPlayer = gameState.getPlayer();
    
    savedPlayer->prepareForNextLevel();
    gameState.setField(GameField(newWidth, newHeight));
    gameState.setPlayer(savedPlayer);
    
    Position center_pos(newWidth/2, newHeight/2);
    savedPlayer->moving(center_pos);

    startLevel();
}

void LevelManager::initializeEnemies(){
    auto& field = gameState.getField();
    auto player = gameState.getPlayer();
    int currentLevel = gameState.getCurrentLevel();
    
    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    mt19937 gen(seed);
    uniform_int_distribution<> dis_x(0, field.getWidth() - 1);
    uniform_int_distribution<> dis_y(0, field.getHeight() - 1);
    
    int enemyCount = 5 + currentLevel;
    for(int i = 0; i < enemyCount; i++){
        Position enemy_pos;
        int attempts = 0;
        do{
            enemy_pos = Position(dis_x(gen), dis_y(gen));
            attempts++;
        }while((enemy_pos == player->getPosition() || !field.isCellFree(enemy_pos) || field.isCellBlocked(enemy_pos)) && attempts < 50);
        
        if(attempts < 50){
            auto enemy = make_shared<Enemy>(enemy_pos.getX(), enemy_pos.getY(), currentLevel);
            gameState.addGameObject(enemy);
        }
    }
}

void LevelManager::initializeTowers() {
    auto& field = gameState.getField();
    auto player = gameState.getPlayer();
    int currentLevel = gameState.getCurrentLevel();
    
    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    mt19937 gen(seed);
    uniform_int_distribution<> dis_x(0, field.getWidth() - 1);
    uniform_int_distribution<> dis_y(0, field.getHeight() - 1);
    
    int towerCount = 2 + currentLevel / 2;
    for(int i = 0; i < towerCount; i++){
        Position tower_pos;
        int attempts = 0;
        do{
            tower_pos = Position(dis_x(gen), dis_y(gen));
            attempts++;
        }while((tower_pos == player->getPosition() || !field.isCellFree(tower_pos) || 
            field.isCellBlocked(tower_pos) || tower_pos.distanceTo(player->getPosition()) < 5) && attempts < 50);
        
        if(attempts < 50){
            auto tower = make_shared<EnemyTower>(tower_pos.getX(), tower_pos.getY(), currentLevel);
            gameState.addGameObject(tower);
        }
    }
}