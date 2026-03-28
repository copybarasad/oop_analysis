#include "ally.hpp"

Ally::Ally(int hp, int maxHP) : Creature(hp, maxHP) {
    Creature::setTeam(Team::Player);
}
Ally::Ally(const Ally &ally) : Creature(ally) {}
Ally::Ally(bytedump bytes) : Creature(bytes) {}

std::string Ally::chr() { return "*"; }
Color Ally::baseColor() { return Color::green(); }
