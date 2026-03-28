#include "SpellAoE.h"
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

int AoESpell::Chebyshev(std::pair<int,int> a, std::pair<int,int> b) const{
    int dx = a.first - b.first;
    if(dx < 0) dx = -dx;
    int dy = a.second - b.second;
    if(dy < 0) dy = -dy;
    return dx > dy ? dx : dy;
}

AoESpell::AoESpell(int range, int damage, int size){
    range_ = range;
    damage_ = damage;
    size_ = size;
    if(size_ < 1) size_ = 1;
}

const char* AoESpell::GetName() const{
    return "Area";
}

bool AoESpell::NeedsTarget() const{
    return true;
}

int AoESpell::GetRange() const{
    return range_;
}

bool AoESpell::CanCast(SpellContext& context){
    if(context.board == 0 || context.player == 0){
        return false;
    }
    if(!context.hasTarget){
        return false;
    }
    if(!IsInside(context.board, context.targetCell)){
        return false;
    }
    std::pair<int,int> pp = context.player->position();
    if(Chebyshev(pp, context.targetCell) > range_){
        return false;
    }
    return true;
}

bool AoESpell::Cast(SpellContext& context){
    if(!CanCast(context)){
        return false;
    }
    int hits = 0;
    for(int dy = 0; dy < size_; ++dy){
        for(int dx = 0; dx < size_; ++dx){
            std::pair<int,int> c = {context.targetCell.first + dx, context.targetCell.second + dy};
            if(!IsInside(context.board, c)){
                continue;
            }
            if(context.enemies != 0){
                for(size_t i = 0; i < context.enemies->size(); ++i){
                    if((*context.enemies)[(int)i].isAlive() && (*context.enemies)[(int)i].position() == c){
                        (*context.enemies)[(int)i].takeDamage(damage_);
                        hits += 1;
                    }
                }
            }
            if(context.base != 0){
                if(c == context.base->position()){
                    context.base->takeDamage(damage_);
                    hits += 1;
                }
            }
            if(context.towers != 0){
                for(size_t i = 0; i < context.towers->size(); ++i){
                    EnemyTower& t = (*context.towers)[(int)i];
                    if(t.isAlive() && t.position() == c){
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
                        hits += 1;
                    }
                }
            }
        }
    }
    (void)hits;
    return true;
}

void AoESpell::ApplyModifier(const SpellModifier& mod){
    range_ += mod.radiusDelta;
    damage_ += mod.damageDelta;
    size_ += mod.aoeSizeDelta;
    if(range_ < 1) range_ = 1;
    if(damage_ < 0) damage_ = 0;
    if(size_ < 1) size_ = 1;
}

bool AoESpell::ConsumesModifier() const {
    return true;
}

ISpell* AoESpell::CloneNew() const{
    return new AoESpell(range_, damage_, size_);
}