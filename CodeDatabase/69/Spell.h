#pragma once
#include "Player.h"
#include "Game.h"
#include "BuffState.h"
#include <string>


class Player;
class Game;
struct Position;

class Spell {
public:
    virtual ~Spell() = default;
    virtual std::string GetName() const = 0;
    virtual bool Apply(Player& caster, Game& game, Position target) = 0;
    virtual void ApplyBuffs(BuffState const& state);
};