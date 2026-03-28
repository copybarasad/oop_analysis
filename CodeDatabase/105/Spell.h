#pragma once

#include <string>

class GameField;
class Player;

class Spell {
public:
    virtual ~Spell() = default;

    virtual bool cast(GameField& field, Player& player, int TargetX, int TargetY) = 0;
    
    virtual const std::string& getName() const = 0;
    virtual const std::string& getDescription() const = 0;
    virtual int getManaCost() const = 0;
    virtual bool requiresTarget() const = 0;
};