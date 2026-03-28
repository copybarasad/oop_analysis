#include "spell_towerbolt.hpp"
#include "game.hpp"
#include <cstdlib>
TowerBoltSpell::TowerBoltSpell(int dmg,int range):damage_(dmg),range_(range){}
const char* TowerBoltSpell::Name() const noexcept { return "TowerBolt"; }
int TowerBoltSpell::Range() const noexcept { return range_; }
bool TowerBoltSpell::Use(Game& game,int tx,int ty){
    int px,py; game.PlayerPos(px,py);
    if (tx!=px || ty!=py) return false;
    int bx,by; game.CurrentActingTowerPos(bx,by);
    if (bx<0 || by<0) return false;
    int d = std::abs(px-bx)+std::abs(py-by);
    if (d>range_) return false;
    game.DamagePlayer(damage_, "Tower", bx, by);
    return true;
}
std::string TowerBoltSpell::Serialize() const {
    return "TOWERBOLT:" + std::to_string(damage_) + ":" + std::to_string(range_);
}
