#pragma once
#include "Entity.h"

struct EntityCompare {
    bool operator()(const Entity* a, const Entity* b) const {
        if (a->move_priority() != b->move_priority())
            return a->move_priority() < b->move_priority();

        return a < b;
    }
};
