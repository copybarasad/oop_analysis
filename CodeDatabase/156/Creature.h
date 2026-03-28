#ifndef LABA1_CREATURE_H
#define LABA1_CREATURE_H
#include "../Entity.h"
#include <string>
#include "../EntityType.h"

class Creature : public Entity {
private:

public:

    Creature(int xCoordinate, int yCoordinate, int healthPoint, EntityType type, bool isDisabledFlag, int stepRange)
            : Entity(xCoordinate, yCoordinate, healthPoint, type), isDisabledFlag(isDisabledFlag), stepRange(stepRange) {}

    Creature(const Creature& other) = default;
    Creature(Creature&& other) noexcept: Entity(std::move(other)), isDisabledFlag(other.isDisabledFlag), stepRange(other.stepRange) {}

    virtual bool isDisabled() noexcept final;

    virtual void setDisabledFlag(bool flag) noexcept final;

    virtual void setXCoordinate(int x) noexcept final;

    virtual void setYCoordinate(int y) noexcept final;

    virtual int getStepRange() const noexcept final;

    void setStepRange(int stepRange);

protected:
    bool isDisabledFlag;
    int stepRange;
};


#endif