#pragma once
#include <memory>
#include "Field.h"
#include "Player.h"

class GameState {
private:
    std::shared_ptr<Field> field_;
    std::shared_ptr<Player> player_;
    int turnCount_;
    int level_;
    int score_;

public:
    GameState();
    GameState(std::shared_ptr<Field> field, std::shared_ptr<Player> player, int level = 1);

    std::shared_ptr<Field> getField() const { return field_; }
    std::shared_ptr<Player> getPlayer() const { return player_; }
    int getTurnCount() const { return turnCount_; }
    int getLevel() const { return level_; }
    int getScore() const { return score_; }

    void setField(std::shared_ptr<Field> field) { field_ = field; }
    void setPlayer(std::shared_ptr<Player> player) { player_ = player; }
    void setLevel(int level) { level_ = level; }
    void setScore(int score) { score_ = score; }
    void incrementTurn() { turnCount_++; }
    
    bool isPlayerAlive() const;
    bool areEnemiesPresent() const;
};