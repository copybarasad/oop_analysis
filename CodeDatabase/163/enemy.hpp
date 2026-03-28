#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "game_object.hpp"
#include "i_attacker.hpp"
#include <memory>

class Player;

class Enemy : public GameObject, public IAttacker {
private:
    int attackDamage;
    Player* targetPlayer;
    bool hasMovedThisTurn;
    bool hasAttackedThisTurn;

    bool findBestMove(Map& map, int& outX, int& outY);
    int calculateDistance(int x1, int y1, int x2, int y2) const;
    void tryAttackPlayer();
    bool tryMoveTowardsPlayer(Map& map);
    void moveRandomly(Map& map);

public:
    Enemy(int startX, int startY, Cell& cell, int hp, int range, 
          int damage, Player* player);
    
    ~Enemy() override = default;
    
    int getAttackDamage() const override;
    bool canAttack() const override;
    void performAttack(GameObject& target) override;
    void resetAttackFlag() override;
    
    void takeTurn(Map& map) override;
    void resetTurnFlags() override;
    
    bool canAttackPlayer() const;
};

#endif