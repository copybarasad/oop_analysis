#pragma once
#include <string>
#include "spellid.h"
class Grid;
class Player;

class ISpell {
public:
    virtual ~ISpell() = default;
    //для уникальности рандомных спеллов
    virtual SpellId id() const = 0;
    //metadata
    virtual const std::string& name() const = 0;
    virtual int radius() const = 0;
    virtual int damage() const = 0;

    //каст по цели на grid
    virtual bool cast(Player& player, Grid& grid, int target_x, int target_y) = 0;
};
