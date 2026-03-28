#pragma once
#include <memory>
#include <string>

class Game;
class PlayerHand;

class Spell {
public:
    virtual ~Spell() = default;

    virtual const std::string& Name() const = 0;
    virtual bool Cast(Game& game, PlayerHand& hand) = 0;

    virtual std::unique_ptr<Spell> Clone() const = 0;
};
