#ifndef UNTITLED_ENEMYBUILDING_H
#define UNTITLED_ENEMYBUILDING_H

#include "Entity.h"

class EnemyBuilding : public Entity {
public:
    EnemyBuilding(int hp = 15, int attackRadius = 3, int attackCooldown = 2);
    ~EnemyBuilding() override = default;

    std::shared_ptr<Entity> clone() const override;

    void onTurn(Field& field, const Coord& pos) override;

    bool tryAttackPlayer(Field& field, Coord pos);

    void decrementTimer();

    void setTurnsUntilAttack(int v);
    int turnsUntilAttack() const;

private:
    int spawnInterval_;
    int attackRadius_;
    int attackCooldown_;
    int turnsUntilAttack_;
};

#endif //UNTITLED_ENEMYBUILDING_H

