#ifndef GAME_H
#define GAME_H

#include <memory>
#include <vector>
#include <string>
#include "../commands/Command.h"

class GameState;
class LevelManager;
class CombatManager;
class ObjectManager;
class SaveLoadManager;
class Player;
class GameField;
class Enemy;
class EnemyTower;
class Trap;

class Game {
private:
    std::unique_ptr<GameState> gameState;
    std::unique_ptr<LevelManager> levelManager;
    std::unique_ptr<CombatManager> combatManager;
    std::unique_ptr<ObjectManager> objectManager;
    std::unique_ptr<SaveLoadManager> saveLoadManager;
    
public:
    Game(int fieldWidth = 15, int fieldHeight = 15, int startLevel = 1);
    ~Game();
    
    void movePlayer(Direction dir);
    void switchWeapon();
    void performRangedAttack(Direction dir);
    void castSpell(int spellIndex, Direction dir);
    void buySpell();
    void saveGame();
    void loadGame();
    void quitGame();
    void nextLevel();
    void restartGame();
    
    GameField& getField();
    std::shared_ptr<Player> getPlayer();
    int getCurrentLevel() const;
    std::vector<std::shared_ptr<Enemy>> getEnemies();
    std::vector<std::shared_ptr<EnemyTower>> getTowers();
    std::vector<std::shared_ptr<Trap>> getTraps();
    GameStatus getStatus() const;
    bool isRunning() const;
    
    void runGame();
    void processEnemyTurns();
    void updateGameState();
};

#endif