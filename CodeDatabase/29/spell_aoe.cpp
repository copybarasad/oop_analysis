#include "spell_aoe.hpp"
#include "game.hpp"
#include <algorithm>
#include <cstdlib>
AoESpell2x2::AoESpell2x2(int dmg,int range):damage_(dmg),range_(range){}
const char* AoESpell2x2::Name() const noexcept { return "AoE2x2"; }
int AoESpell2x2::Range() const noexcept { return range_; }
bool AoESpell2x2::Use(Game& game,int tx,int ty){
    int px,py; game.PlayerPos(px,py);
    int d = std::abs(px-tx)+std::abs(py-ty);
    if (d>range_) return false;
    int W = game.BoardRef().Width(), H = game.BoardRef().Height();
    int bx = std::max(0, std::min(tx, W-2));
    int by = std::max(0, std::min(ty, H-2));
    for(int y=by;y<by+2;++y){
        for(int x=bx;x<bx+2;++x){
            game.DamageEnemyAt(x,y,damage_);
            game.DamageTowerAt(x,y,damage_);
        }
    }
    return true;
}
std::string AoESpell2x2::Serialize() const {
    return "AOE2X2:" + std::to_string(damage_) + ":" + std::to_string(range_);
}
