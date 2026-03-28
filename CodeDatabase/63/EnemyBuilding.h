#ifndef RPG_ENEMYBUILDING_H
#define RPG_ENEMYBUILDING_H

#include "Entity.h"
#include "SpellsHand.h"

class EnemyBuilding : public Entity {
public:
    EnemyBuilding(unsigned int h, unsigned int d, unsigned int castDiameter, unsigned int cooldown);

    explicit EnemyBuilding(json& data);

    [[nodiscard]] std::unique_ptr<Entity> clone() const override;

    void onEnter(Entity *aggressor) override;

    StepInfo move(StepRequirements& arguments) override;
};

#endif
