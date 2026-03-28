#pragma once
#include "Entity.h"
#include "struct/dataAlly.h"


class Ally : public Entity {
public:
    Ally(int health, int damage) : Entity(health, damage) {}
    Ally(dataAlly data);
    ~Ally() override = default;

    dataAlly Save();
};