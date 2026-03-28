#include "SpellDirect.h"
#include "Board.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBase.h"
#include "EnemyTower.h"
#include "Config.h"
#include "EmpowerSpell.h"

static bool IsInside(Board* b, std::pair<int,int> p){
    if(b == 0){
        return false;
    }
    return p.first >= 0 && p.second >= 0 && p.first < b->width() && p.second < b->height();
}

int DirectDamageSpell::Chebyshev(std::pair<int,int> a, std::pair<int,int> b) const{
    int dx = a.first - b.first;
    if(dx < 0) dx = -dx;
    int dy = a.second - b.second;
    if(dy < 0) dy = -dy;
    return dx > dy ? dx : dy;
}

DirectDamageSpell::DirectDamageSpell(int range, int damage){
    range_ = range;
    damage_ = damage;
}

const char* DirectDamageSpell::GetName() const{
    return "Direct Spell";
}

bool DirectDamageSpell::NeedsTarget() const{
    return true;
}

int DirectDamageSpell::GetRange() const{
    return range_;
}

bool DirectDamageSpell::CanCast(SpellContext& context){
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

    if(context.enemies != 0){
        for(size_t i = 0; i < context.enemies->size(); ++i){
            Enemy& e = (*context.enemies)[(int)i];
            if(e.isAlive() && e.position() == context.targetCell){
                return true;
            }
        }
    }
    if(context.towers != 0){
        for(size_t i = 0; i < context.towers->size(); ++i){
            EnemyTower& t = (*context.towers)[(int)i];
            if(t.isAlive() && t.position() == context.targetCell){
                bool wasAlive = t.isAlive();
                t.takeDamage(damage_);
                if(wasAlive && !t.isAlive() && context.player != 0){
                    if(context.player->hand().GetCount() < context.player->hand().GetCapacity()){
                        context.player->hand().AddCard(
                            new EmpowerSpell(EMPOWER_RADIUS_DELTA, EMPOWER_AOE_DELTA, EMPOWER_DAMAGE_DELTA, EMPOWER_SUMMON_DELTA)
                        );
                    }else{
                        SpellModifier gain;
                        gain.radiusDelta = EMPOWER_RADIUS_DELTA;
                        gain.aoeSizeDelta = EMPOWER_AOE_DELTA;
                        gain.damageDelta = EMPOWER_DAMAGE_DELTA;
                        gain.summonCountDelta = EMPOWER_SUMMON_DELTA;
                        context.player->AddPendingModifier(gain);
                    }
                }
                return true;
            }
        }
    }
    if(context.base != 0){
        if(context.targetCell == context.base->position()){
            return true;
        }
    }
    return false;
}

bool DirectDamageSpell::Cast(SpellContext& context){
    if(!CanCast(context)){
        return false; 
    }

    if(context.enemies != 0){
        for(size_t i = 0; i < context.enemies->size(); ++i){
            Enemy& e = (*context.enemies)[(int)i];
            if(e.isAlive() && e.position() == context.targetCell){
                e.takeDamage(damage_);
                return true;
            }
        }
    }

    if(context.towers != 0){
        for(size_t i = 0; i < context.towers->size(); ++i){
            EnemyTower& t = (*context.towers)[(int)i];
            if(t.isAlive() && t.position() == context.targetCell){
                t.takeDamage(damage_);
                return true;
            }
        }
    }
    if(context.base != 0){
        if(context.targetCell == context.base->position()){
            context.base->takeDamage(damage_);
            return true;
        }
    }

    return false;
}

void DirectDamageSpell::ApplyModifier(const SpellModifier& mod){
    range_ += mod.radiusDelta;
    damage_ += mod.damageDelta;
    if(range_ < 1) range_ = 1;
    if(damage_ < 0) damage_ = 0;
}

bool DirectDamageSpell::ConsumesModifier() const {
    return true;
}

bool DirectDamageSpell::IsPersistent() const {
    return false; 
}

ISpell* DirectDamageSpell::CloneNew() const{
    return new DirectDamageSpell(range_, damage_);
}