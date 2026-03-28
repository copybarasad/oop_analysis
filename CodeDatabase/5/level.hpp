#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "game_field.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "spell_hand.hpp"
#include "spell_factory.hpp"
#include <vector>
#include <iostream>

class Game;

class Level {
private:
    int levelNumber;
    GameField field;
    Player player;
    std::vector<Enemy> enemies;
    Hand spellHand;
    bool completed;
    bool failed;
    Game* game;

public:
    Level(Game* parentGame,int number, int width, int height);
    bool run();
    bool isCompleted() const { return completed; }
    bool isFailed() const { return failed; }
    void reset();
    const Player& getPlayer() const { return player; }
    Player& getPlayer() { return player; }
    const std::vector<Enemy>& getEnemies() const { return enemies; }
    std::vector<Enemy>& getEnemies() { return enemies; }
    const Hand& getSpellHand() const { return spellHand; }
    Hand& getSpellHand() { return spellHand; }
    int getLevelNumber() const { return levelNumber; }
    
private:
    void processEnemyTurns();
    void displayStatus() const;
    void displayEnemyInfo() const;
    bool allEnemiesDead() const;
    void offerSpellPurchase();
};

#endif