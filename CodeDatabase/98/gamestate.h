#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "field.h"
#include "player.h"
#include "enemy.h"
#include "hand.h"
#include <vector>

class GameState {
private:
    // Игрок
    Player player;
    int playerX;
    int playerY;

    // Враги
    std::vector<Enemy> enemies;
    std::vector<std::pair<int,int>> enemyPositions;

    // Заклинания
    Hand hand;

    // Уровень
    int currentLevel;

    int fieldWidth;
    int fieldHeight;
    double blockChance;
    int baseEnemyCount;

public:
    GameState();
    Player& GetPlayer();
    const Player& GetPlayer() const;

    int GetPlayerX() const;
    int GetPlayerY() const;

    std::vector<Enemy>& GetEnemies();
    const std::vector<Enemy>& GetEnemies() const;

    std::vector<std::pair<int,int>>& GetEnemyPositions();
    const std::vector<std::pair<int,int>>& GetEnemyPositions() const;

    Hand& GetHand();
    const Hand& GetHand() const;

    int GetCurrentLevel() const;

    int GetFieldWidth() const;
    int GetFieldHeight() const;
    double GetBlockChance() const;
    int GetBaseEnemyCount() const;

    void SetPlayer(const Player& newPlayer);
    void SetPlayerPosition(int x, int y);

    void SetEnemies(const std::vector<Enemy>& newEnemies);
    void SetEnemyPositions(const std::vector<std::pair<int,int>>& positions);

    void SetHand(const Hand& newHand);

    void SetCurrentLevel(int level);

    void SetFieldSize(int w, int h);
    void SetBlockChance(double chance);
    void SetBaseEnemyCount(int value);
};

#endif
