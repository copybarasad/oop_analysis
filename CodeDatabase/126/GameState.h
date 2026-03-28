#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <memory>
#include <vector>
#include "../field/GameField.h"
#include "../enums/Enums.h"

class Player;
class GameObject;
class Enemy;
class EnemyTower;
class Trap;

class GameState {
private:
    GameField field;
    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    GameStatus status;
    int currentLevel;
    bool gameRunning;
    
public:
    GameState(int fieldWidth, int fieldHeight, int startLevel);
    
    GameField& getField();
    std::shared_ptr<Player> getPlayer();
    std::vector<std::shared_ptr<GameObject>>& getGameObjects();
    GameStatus getStatus() const;
    int getCurrentLevel() const;
    bool isRunning() const;
    
    void setStatus(GameStatus newStatus);
    void setGameRunning(bool running);
    void setCurrentLevel(int level);
    void setPlayer(const std::shared_ptr<Player>& newPlayer);
    void setField(const GameField& newField);
    
    void clearGameObjects();
    void addGameObject(std::shared_ptr<GameObject> obj);
    void updateFieldState();
    
    std::vector<std::shared_ptr<Enemy>> getEnemies();
    std::vector<std::shared_ptr<EnemyTower>> getTowers();
    std::vector<std::shared_ptr<Trap>> getTraps();
};

#endif