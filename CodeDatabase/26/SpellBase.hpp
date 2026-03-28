#pragma once
#include <string>
#include "Position.hpp"

class GameWorld;
class Player;

class SpellBase {
public:
    struct SpellContext {
        GameWorld& world;
        Player&    player;
        Position   targetCell;
        bool       hasTarget;
    };
    virtual std::string name() const = 0;
    virtual int hotkey() const noexcept = 0;
    virtual bool requiresTarget() const noexcept = 0;
    virtual bool cast(SpellContext& ctx) = 0;
    virtual ~SpellBase() {}
};
