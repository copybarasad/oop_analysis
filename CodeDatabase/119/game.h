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
#include "game_saver.h"
#include "game_loader.h"
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
    GameSaver saver;
    GameLoader loader;
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
    void startNewGame();
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
    
    // Getters для сохранения/загрузки
    int getCurrentLevel() const { return currentLevel; }
    void setCurrentLevel(int level) { currentLevel = level; }
    GameField& getField() { return field; }
    const GameField& getField() const { return field; }
    Player& getPlayer() { return player; }
    const Player& getPlayer() const { return player; }
    Enemy& getEnemy() { return enemy; }
    const Enemy& getEnemy() const { return enemy; }
    Enemy& getEnemy2() { return enemy2; }
    const Enemy& getEnemy2() const { return enemy2; }
    EnemyTower& getEnemyTower() { return enemy_tower; }
    const EnemyTower& getEnemyTower() const { return enemy_tower; }
};
#endif