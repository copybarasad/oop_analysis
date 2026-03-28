#include "spell_trap.hpp"
#include "game.hpp"
#include <cstdlib>
TrapSpell::TrapSpell(int dmg,int range):damage_(dmg),range_(range){}
const char* TrapSpell::Name() const noexcept { return "Trap"; }
int TrapSpell::Range() const noexcept { return range_; }
bool TrapSpell::Use(Game& game,int tx,int ty){
    int px,py; game.PlayerPos(px,py);
    int d = std::abs(px-tx)+std::abs(py-ty);
    if (d>range_) return false;
    if (!game.BoardRef().InBounds(tx,ty)) return false;
    if (!game.BoardRef().IsPassable(tx,ty)) return false;
    if (game.HasTrapAt(tx,ty)) return false;
    if (game.EnemyAt(tx,ty)) return false;
    game.PlaceTrap(tx,ty,damage_);
    return true;
}
std::string TrapSpell::Serialize() const {
    return "TRAP:" + std::to_string(damage_) + ":" + std::to_string(range_);
}
