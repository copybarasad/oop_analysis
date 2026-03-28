#pragma once
#include <string>

class Gamefield;
class Player;

class Spell {
public:
    virtual ~Spell() = default;

    virtual const std::string& name() const = 0;
    virtual int radius() const = 0;

    virtual bool cast(Player& player, Gamefield& field, int target_x, int target_y) = 0;
    virtual void upgrade() = 0;
    virtual int get_power() const = 0;
    virtual int get_type_id() const = 0;
};
