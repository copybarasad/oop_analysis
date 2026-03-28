#pragma once

#include <set>
#include "effect.hpp"
#include "IGameObject.hpp"

class ILandscape {
public:
    virtual std::set<Effect> step() = 0;
    virtual bool trampled() = 0;
};