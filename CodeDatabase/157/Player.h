#ifndef PLAYER_H
#define PLAYER_H

#include "Hand.h"

class Unit;
class GameField;

class Player {
public:
    Player(int x, int y, GameField& field, size_t handCapacity);
    int x() const { return x_; }
    int y() const { return y_; }
    void moveTo(int nx, int ny);
    void spendTurn() { hasMoved_ = true; }
    void nextTurn() { hasMoved_ = false; }
    bool hasMoved() const { return hasMoved_; }
    Hand& hand() { return hand_; }
    GameField& field() { return field_; }
    void takeDamage(int d);
    void onEnemyKilled();
    void attack(Unit* u);
    int hp() const { return hp_; }
    int damage() const { return damage_; }
    int score() const { return score_; }
    void setHp(int h) { hp_ = h; }
    int kills() const { return kills_; }
    void setScore(int s) { score_ = s; }
    void setKills(int k) { kills_ = k; }
private:
    int x_, y_;
    bool hasMoved_;
    Hand hand_;
    GameField& field_;
    int hp_ = 100;
    int damage_ = 5;
    int score_ = 0;
    int kills_ = 0;
};
#endif // PLAYER_H
