#ifndef RPG_TRAP_H
#define RPG_TRAP_H

#include "Entity.h"

class Trap : public Entity {
    unsigned int lifetime;
public:
    Trap(unsigned int d, EntityTeam team);

    explicit Trap(json& data);

    [[nodiscard]] std::unique_ptr<Entity> clone() const override;

    [[nodiscard]] json to_json() const override;

    StepInfo move(StepRequirements& arguments) override;

    void onEnter(Entity *aggressor) override;
};


#endif