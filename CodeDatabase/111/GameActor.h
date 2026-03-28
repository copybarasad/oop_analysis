#pragma once
#include "Location.h"
#include "CombatAttributes.h"
#include "MoveDirection.h"

class GameBoard;
enum class MoveDirection;

class GameActor {
protected:
    CombatAttributes attributes;
    Location position;

public:
    GameActor(int hitpoints, int attack, const Location& pos);
    virtual ~GameActor() = default;

    virtual void performMove(MoveDirection dir, GameBoard& board) = 0;
    virtual void handleCollision(GameActor* other) = 0;
    
    void receiveDamage(int damage);
    
    Location getPosition() const;
    int getHitpoints() const;
    int getMaxHitpoints() const;
    int getAttackPower() const;
    bool isAlive() const;
    
    void setPosition(const Location& newPos);

    void setHitpoints(int hp) {
    attributes.hitpoints = hp;
}
};