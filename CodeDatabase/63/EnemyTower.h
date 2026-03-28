#ifndef RPG_ENEMYTOWER_H
#define RPG_ENEMYTOWER_H

#include "Entity.h"
#include "SpellsHand.h"

class EnemyTower : public Entity {
public:
    EnemyTower(unsigned int h, unsigned int d, unsigned int attackDiameter, unsigned int cooldown);

    explicit EnemyTower(json& data);

    [[nodiscard]] std::unique_ptr<Entity> clone() const override;

    void onEnter(Entity *aggressor) override;

    StepInfo move(StepRequirements& arguments) override;
};


#endif