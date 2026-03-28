#ifndef GAME4_H
#define GAME4_H

#include <memory>
#include <string>
#include "GameField.h"
#include "Player.h"

class Game4 {
public:
    Game4();

    Game4(const Game4&) = delete;
    Game4& operator=(const Game4&) = delete;

    void run();

private:
    void startNewGame();
    void startLevel();
    void gameLoop();
    void processCommand(const std::string &line);

    void cmdMove(int x,int y);
    void cmdUse(size_t idx,int tx,int ty);
    void cmdSave(const std::string &fn);
    void cmdLoad(const std::string &fn);
    void cmdStatus();

    void nextLevel();

    int countEnemyUnits() const;
    bool anyEnemyLeft() const;

private:
    std::unique_ptr<GameField> field_;
    std::unique_ptr<Player> player_;
    int levelNumber_ = 1;
    int baseWidth_ = 10;
    int baseHeight_ = 10;
};

#endif