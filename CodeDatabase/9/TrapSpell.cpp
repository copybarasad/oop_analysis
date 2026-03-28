#include "TrapSpell.h"
#include "Board.h"
#include "Cell.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBase.h"
#include "EnemyTower.h"

static bool IsInside(Board* b, std::pair<int,int> p){
    if(b == 0){
        return false;
    }
    return p.first >= 0 && p.second >= 0 && p.first < b->width() && p.second < b->height();
}

int TrapSpell::Chebyshev(std::pair<int,int> a, std::pair<int,int> b) const{
    int dx = a.first - b.first; if(dx < 0) dx = -dx;
    int dy = a.second - b.second; if(dy < 0) dy = -dy;
    return dx > dy ? dx : dy;
}

TrapSpell::TrapSpell(int range, int damage){
    range_ = range;
    damage_ = damage;
}

const char* TrapSpell::GetName() const{
    return "Trap";
}

bool TrapSpell::NeedsTarget() const{
    return true;
}

int TrapSpell::GetRange() const{
    return range_;
}

bool TrapSpell::CanCast(SpellContext& context){
    if(context.board == 0 || context.player == 0){
        return false;
    }
    if(!context.hasTarget){
        return false;
    }
    if(!IsInside(context.board, context.targetCell)){
        return false;
    }
    if(Chebyshev(context.player->position(), context.targetCell) > range_){
        return false;
    }
    Cell& c = context.board->at(context.targetCell);
    if(!c.isPassable()){
        return false;
    }
    if(c.hasTrap()){
        return false;
    }
    if(context.player->position() == context.targetCell){
        return false;
    }
    if(context.base != 0 && context.base->position() == context.targetCell){
        return false;
    }
    if(context.enemies != 0){
        for(size_t i = 0; i < context.enemies->size(); ++i){
            if((*context.enemies)[(int)i].isAlive() && (*context.enemies)[(int)i].position() == context.targetCell){
                return false;
            }
        }
    }
    if(context.towers != 0){
        for(size_t i = 0; i < context.towers->size(); ++i){
            if((*context.towers)[(int)i].isAlive() && (*context.towers)[(int)i].position() == context.targetCell){
                return false;
            }
        }
    }
    return true;
}

bool TrapSpell::Cast(SpellContext& context){
    if(!CanCast(context)){
        return false;
    }
    Cell& c = context.board->at(context.targetCell);
    c.setTrap(damage_);
    return true;
}

void TrapSpell::ApplyModifier(const SpellModifier& mod){
    range_ += mod.radiusDelta;
    damage_ += mod.damageDelta;
    if(range_ < 1) range_ = 1;
    if(damage_ < 0) damage_ = 0;
}

bool TrapSpell::ConsumesModifier() const {
    return true;
}

bool TrapSpell::IsPersistent() const {
    return false; 
}

ISpell* TrapSpell::CloneNew() const{
    return new TrapSpell(range_, damage_);
}