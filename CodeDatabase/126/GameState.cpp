#include "../../headers/gamelogic/GameState.h"
#include "../../headers/objects/Player.h"
#include "../../headers/objects/Enemy.h"
#include "../../headers/objects/EnemyTower.h"
#include "../../headers/objects/Trap.h"
#include <memory>

using namespace std;

GameState::GameState(int fieldWidth, int fieldHeight, int startLevel) 
    : field(fieldWidth, fieldHeight), currentLevel(startLevel), gameRunning(true) {
    Position center_pos(fieldWidth / 2, fieldHeight / 2);
    player = make_shared<Player>(fieldWidth/2, fieldHeight/2, 3, startLevel);
    player->moving(center_pos);
}

GameField& GameState::getField() { return field; }

shared_ptr<Player> GameState::getPlayer() { return player; }

vector<shared_ptr<GameObject>>& GameState::getGameObjects() { return gameObjects; }

GameStatus GameState::getStatus() const { return status; }

int GameState::getCurrentLevel() const { return currentLevel; }

bool GameState::isRunning() const { return gameRunning; }

void GameState::setStatus(GameStatus newStatus) { status = newStatus; }

void GameState::setGameRunning(bool running) { gameRunning = running; }

void GameState::setCurrentLevel(int level) { currentLevel = level; }

void GameState::setPlayer(const shared_ptr<Player>& newPlayer) { player = newPlayer; }

void GameState::setField(const GameField& newField) { field = newField; }

void GameState::clearGameObjects() { gameObjects.clear(); }

void GameState::addGameObject(shared_ptr<GameObject> obj) { gameObjects.push_back(obj); }

void GameState::updateFieldState() {
    for(int y = 0; y < field.getHeight(); y++){
        for(int x = 0; x < field.getWidth(); x++){
            Position pos(x, y);
            field.clearCell(pos);
            field.removeTrapPosition(pos);
        }
    }
    
    field.setPlayerPosition(player->getPosition());
    
    for(const auto& enemy : getEnemies()){
        if (enemy->isAlive()){
            field.setEnemyPosition(enemy->getPosition());
        }
    }
    
    for(const auto& tower : getTowers()){
        if (tower->isAlive()){
            field.setTowerPosition(tower->getPosition());
        }
    }
    
    for(const auto& trap : getTraps()){
        if (trap->isAlive()){
            field.setTrapPosition(trap->getPosition());
        }
    }
}

vector<shared_ptr<Enemy>> GameState::getEnemies() {
    vector<shared_ptr<Enemy>> result;
    for (auto& obj : gameObjects) {
        if (auto enemy = dynamic_pointer_cast<Enemy>(obj)) {
            if (enemy->isAlive()) {
                result.push_back(enemy);
            }
        }
    }
    return result;
}

vector<shared_ptr<EnemyTower>> GameState::getTowers() {
    vector<shared_ptr<EnemyTower>> result;
    for (auto& obj : gameObjects) {
        if (auto tower = dynamic_pointer_cast<EnemyTower>(obj)) {
            if (tower->isAlive()) {
                result.push_back(tower);
            }
        }
    }
    return result;
}

vector<shared_ptr<Trap>> GameState::getTraps() {
    vector<shared_ptr<Trap>> result;
    for (auto& obj : gameObjects) {
        if (auto trap = dynamic_pointer_cast<Trap>(obj)) {
            if (trap->isAlive()) {
                result.push_back(trap);
            }
        }
    }
    return result;
}