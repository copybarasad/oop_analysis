#include "SummonSpell.h"
#include "Board.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBase.h"
#include "EnemyTower.h"
#include "Ally.h"
#include "Config.h"

static bool IsInside(Board* b, std::pair<int,int> p){
    if(b == 0){
        return false;
    }
    return p.first >= 0 && p.second >= 0 && p.first < b->width() && p.second < b->height();
}

SummonSpell::SummonSpell(int count){
    count_ = count;
    if(count_ < 1) count_ = 1;
}

const char* SummonSpell::GetName() const{
    return "Summon";
}

bool SummonSpell::NeedsTarget() const{
    return false;
}

int SummonSpell::GetRange() const{
    return 1;
}

bool SummonSpell::CanCast(SpellContext& context){
    if(context.board == 0 || context.allies == 0){
        return false;
    }
    std::pair<int,int> p = context.player->position();
    std::pair<int,int> dirs[4] = {{1,0},{-1,0},{0,1},{0,-1}};
    for(int i = 0; i < 4; ++i){
        std::pair<int,int> c = {p.first + dirs[i].first, p.second + dirs[i].second};
        if(!IsInside(context.board, c)) continue;
        if(!context.board->at(c).isPassable()) continue;
        bool busy = false;
        if(context.base != 0 && context.base->position() == c) busy = true;
        if(!busy && context.enemies != 0){
            for(size_t j = 0; j < context.enemies->size(); ++j){
                if((*context.enemies)[(int)j].isAlive() && (*context.enemies)[(int)j].position() == c){ busy = true; break; }
            }
        }
        if(!busy && context.towers != 0){
            for(size_t j = 0; j < context.towers->size(); ++j){
                if((*context.towers)[(int)j].isAlive() && (*context.towers)[(int)j].position() == c){ busy = true; break; }
            }
        }
        if(!busy && context.allies != 0){
            for(size_t j = 0; j < context.allies->size(); ++j){
                if((*context.allies)[(int)j].isAlive() && (*context.allies)[(int)j].position() == c){ busy = true; break; }
            }
        }
        if(!busy){
            return true;
        }
    }
    return false;
}

bool SummonSpell::Cast(SpellContext& context){
    if(!CanCast(context)){
        return false;
    }
    std::pair<int,int> p = context.player->position();
    std::pair<int,int> dirs[4] = {{1,0},{-1,0},{0,1},{0,-1}};
    int placed = 0;

    for(int k = 0; k < count_; ++k){
        bool spawned = false;
        for(int i = 0; i < 4; ++i){
            std::pair<int,int> c = {p.first + dirs[i].first, p.second + dirs[i].second};
            if(!IsInside(context.board, c)) continue;
            if(!context.board->at(c).isPassable()) continue;

            bool busy = false;
            if(context.base != 0 && context.base->position() == c) busy = true;
            if(!busy && context.enemies != 0){
                for(size_t j = 0; j < context.enemies->size(); ++j){
                    if((*context.enemies)[(int)j].isAlive() && (*context.enemies)[(int)j].position() == c){ busy = true; break; }
                }
            }
            if(!busy && context.towers != 0){
                for(size_t j = 0; j < context.towers->size(); ++j){
                    if((*context.towers)[(int)j].isAlive() && (*context.towers)[(int)j].position() == c){ busy = true; break; }
                }
            }
            if(!busy && context.allies != 0){
                for(size_t j = 0; j < context.allies->size(); ++j){
                    if((*context.allies)[(int)j].isAlive() && (*context.allies)[(int)j].position() == c){ busy = true; break; }
                }
            }

            if(!busy){
                context.allies->push_back(Ally(ALLY_HP, ALLY_DAMAGE, c));
                spawned = true;
                placed += 1;
                break;
            }
        }
        if(!spawned){
            break;
        }
    }
    return placed > 0;
}

void SummonSpell::ApplyModifier(const SpellModifier& mod){
    count_ += mod.summonCountDelta;
    if(count_ < 1) count_ = 1;
}

bool SummonSpell::ConsumesModifier() const{
    return true;
}

bool SummonSpell::IsPersistent() const {
    return false; 
}

ISpell* SummonSpell::CloneNew() const{
    return new SummonSpell(count_);
}