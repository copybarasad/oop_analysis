#pragma once

#include <cstddef>
#include <string>

class Game;

class Spell {
public:
    virtual ~Spell() = default;

    virtual std::string GetName() const = 0;

    virtual int GetCost() const = 0;

    virtual bool CanCast(const Game &game,
                         std::size_t target_x,
                         std::size_t target_y) const = 0;

    virtual bool Cast(Game &game,
                      std::size_t target_x,
                      std::size_t target_y) = 0;
};
