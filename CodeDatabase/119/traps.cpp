#include <iostream>
#include <cmath>
#include "game_field.h"
#include "traps.h"
#include "prints.h"

Traps::Traps(int dmg, std::string name, SpellType id)
    : dmg(dmg), name(name), id(id) {}

void Traps::cast(const CastContext &context) const
{
    GameField* field = Entity::currentField;
    Cell& c = field->getCell(context.targetY, context.targetX + 1);

    if(c.getType() == IMPASSABLE || c.hasEntity()){
        Prints::print_trap_destroyed_string();
    }
    else{
        c = Cell(TRAPS);
        Prints::print_trap_placed_string();
    }
}

void Traps::upgrade()
{
    dmg += 15;
}

std::string Traps::getName() const { return name; }
