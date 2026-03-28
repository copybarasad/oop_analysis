#include "spell_direct.hpp"
#include "game.hpp"
#include <cstdlib>
DirectDamageSpell::DirectDamageSpell(int dmg,int range):damage_(dmg),range_(range){}
const char* DirectDamageSpell::Name() const noexcept { return "Direct"; }
int DirectDamageSpell::Range() const noexcept { return range_; }
bool DirectDamageSpell::Use(Game& game,int tx,int ty){
    int px,py; game.PlayerPos(px,py);
    int d = std::abs(px-tx)+std::abs(py-ty);
    if (d>range_) return false;
    if (game.DamageEnemyAt(tx,ty,damage_)) return true;
    if (game.DamageTowerAt(tx,ty,damage_)) return true;
    return false;
}
std::string DirectDamageSpell::Serialize() const {
    return "DIRECT:" + std::to_string(damage_) + ":" + std::to_string(range_);
}
