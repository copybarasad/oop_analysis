#ifndef ENEMYTOWER_H
#define ENEMYTOWER_H

#include "position.h"
#include "spell.h"
#include <memory>

class Player;

class EnemyTower {
private:
    Position position;
    int attackRadius;
    std::unique_ptr<Spell> attackSpell;
    bool canAttack;
    int cooldownTimer;
    int cooldownDuration;
    
public:
    EnemyTower(const Position& position, int attackRadius = 3, int cooldownDuration = 2);
    EnemyTower(const EnemyTower& other);
    EnemyTower(EnemyTower&& other) = default;
    EnemyTower& operator=(const EnemyTower& other);
    EnemyTower& operator=(EnemyTower&& other) = default;
    
    // Lab requirement: "если в радиусе ее атаки появляется игрок, то применяет ослабленную версию заклинания прямого урона"
    bool attackPlayer(Player* player, GameField* field);
    
    // Lab requirement: "Не может применять заклинание несколько ходов подряд"
    void updateCooldown();
    bool isPlayerInRange(const Position& playerPosition) const;
    
    const Position& getPosition() const;
    int getAttackRadius() const;
    bool getCanAttack() const;
};

#endif