#ifndef GAME_H
#define GAME_H

#include "cell.h"
#include "entity.h"
#include "enemy.h"
#include "player.h"
#include "game_field.h"
#include "enemy_tower.h"
#include "hand.h"
#include "spell_creation.h"
#include "prints.h"
#include <random>

class Game {
private:
    GameField field;
    int currentLevel;
    Player player;
    Enemy enemy;
    Enemy enemy2;
    EnemyTower enemy_tower;
    PlayerHand hand;
    
    std::mt19937 gen;
    void generateLevel();
    void nextLevel();
    void handlePlayerInput(const std::string& move);
    void processEnemyTurn();
    void processTowerTurn();
    void checkGameEnd();
    void startUpgrades();
    
    
public:
    Game();
    bool isGameOver() const;
    void update(); 
    //void run();
    void startNewGame();
    void renderGameStatus() const;
    void saveGame(const std::string &filename);
    void loadGame(const std::string &filename);
    void updateWorld();
    void playerAttack();
    void movePlayer(int dx, int dy);

    void switchPlayerMode();
    void playerCastSpell(int spellIndex);
    void playerBuySpell();

    int getFieldWidth() const { return field.getSizeX(); }
    int getFieldHeight() const { return field.getSizeY(); }
    
    int getPlayerX() const { return player.getPosX(); }
    int getPlayerY() const { return player.getPosY(); }
    int getPlayerHp() const { return player.getHp(); }
    int getPlayerXp() const { return player.getXp(); }

    int getEnemyHp() const { return enemy.getHp(); }
    bool isEnemyAlive() const { return enemy.isAlive(); }
    
    // Вспомогательные проверки для отрисовки
    bool isWall(int x, int y) const { return field.getCell(y, x).getType() == IMPASSABLE; } 
    bool isEnemyAt(int x, int y) const {
        return (enemy.isAlive() && enemy.getPosX() == x && enemy.getPosY() == y) ||
               (enemy2.isAlive() && enemy2.getPosX() == x && enemy2.getPosY() == y);
    }
    bool isTowerAt(int x, int y) const {
        return enemy_tower.isAlive() && enemy_tower.getPosX() == x && enemy_tower.getPosY() == y;
    }
};
#endif