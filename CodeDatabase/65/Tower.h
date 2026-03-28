#pragma once

#include "Enemy.h"
#include "GameObject.h"
#include <memory>

class Tower : public Enemy {
    int stepBeforeSpawn;
    int currentStep = 1;
public:
    Tower(int x, int y, char sprite = 'T', std::string color = "\033[31m",
          int health = 10, int damage = 0, int speed = 0, int aggroRange = 2, 
          ObjectTag tags = ObjectTag::TAG_TOWER, int stepBeforeSpawn = 10) 
        : Enemy(x, y, sprite, color, health, damage, speed, aggroRange, tags),
          stepBeforeSpawn(stepBeforeSpawn) {}

    int getStepBeforeSpawn() const { return stepBeforeSpawn; }
    int getCurrentStep() const { return currentStep; }

    void setCurrentStep(int newStep) {
        if (newStep < 0) return;
        currentStep = newStep;
    }

    std::unique_ptr<GameObject> clone() const override {
        auto t = std::make_unique<Tower>(
            x, y, sprite, color,
            health, damage, speed, getAggroRange(),
            static_cast<ObjectTag>(tags), stepBeforeSpawn);
        t->setCurrentStep(currentStep);
        t->setCanMove(canMove);
        return t;
    }
};
