#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include <vector>

class GameState {
private:
    GameField field;
    Player player;
    std::vector<Enemy> enemies;
    bool playerTurn;

public:
    GameState();

    void initializeNewGame();
    void initializeTestGame();

    bool isPlayerTurn() const { return playerTurn; }
    void endPlayerTurn() { playerTurn = false; }
    void endEnemyTurn() { playerTurn = true; }
    void resetTurns() { playerTurn = true; }

    bool checkPlayerVictory() const;
    bool isPlayerAlive() const { return player.isAlive(); }
    bool isValidMove(int newX, int newY) const;
    bool canPlayerCastSpell(int spellIndex) const;

    GameField& getField() { return field; }
    Player& getPlayer() { return player; }
    std::vector<Enemy>& getEnemies() { return enemies; }

    const GameField& getField() const { return field; }
    const Player& getPlayer() const { return player; }
    const std::vector<Enemy>& getEnemies() const { return enemies; }
};

#endif