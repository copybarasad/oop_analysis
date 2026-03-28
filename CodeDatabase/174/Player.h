#ifndef PLAYER_H
#define PLAYER_H

#include "Unit.h"
#include "Direction.h"
#include "PlayerSpellHand.h"
#include <vector>
#include <memory>

class Enemy;
class GameField;
class EnemyBuilding;

class Player : public Unit {
private:
    int scorePoints;
    char fightingStyle;
    int damageBonus;
    int attackRange;
    bool skipNextMove;
    PlayerSpellHand hand;
    

public:
    Player(int x, int y);
    
    int getScorePoints() const;
    char getFightingStyle() const;
    int getAttackRange() const;
    int getDamage() const;
    int getDamageBonus() const;
    PlayerSpellHand& getHand();
    const PlayerSpellHand& getHand() const;
    bool shouldSkipMove() const;
    
    void resetSkipMove();
    void addScorePoints(int points);
    void setFightingStyle(char newFightingStyle);
    void setDamageBonus(int bonus);
    void setSkipMove(bool skip);

    bool move(Direction direction, int maxWidth, int maxHeight, 
              const std::vector<std::shared_ptr<Enemy>>& enemies,
              const std::vector<std::shared_ptr<EnemyBuilding>>& buildings, 
              const GameField& field);
    
    CellType getCellType() const;
};

#endif