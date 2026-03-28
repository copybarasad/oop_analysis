#pragma once
#include "GameActor.h"
#include "DirectDamageSpell.h"

class EnemyTower : public GameActor {
private:
    DirectDamageSpell attackSpell;
    bool canAttackThisTurn;
    int attackRange;

public:
    EnemyTower(int hitpoints, int attack, const Location& pos, int range);
    
    void performMove(MoveDirection dir, GameBoard& board) override;
    void handleCollision(GameActor* other) override;
    
    void performTurn(GameBoard& board, const Location& heroPosition);
    bool isInAttackRange(const Location& target) const;
    void resetAttackCooldown() { canAttackThisTurn = true; }
    
    int getAttackRange() const { return attackRange; }
    
    // Методы для получения урона (наследуются от GameActor)
    using GameActor::receiveDamage;
    using GameActor::isAlive;
    using GameActor::getHitpoints;
    using GameActor::getMaxHitpoints;
    using GameActor::getPosition;
};