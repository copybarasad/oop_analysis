#include <vector>
#include <utility>
#include <memory>
#include <cmath>
#include "DirectDamageSpell.h"
#include "EnemyManager.h"
#include "BuildingManager.h"
#include "PlayerManager.h"
#include "AllyManager.h"
#include "TowerManager.h"

bool DirectDamageSpell::Cast(SpellContext& ctx) {
    if(!ctx.field.inBounds(ctx.target) || ctx.target == ctx.playerPos) {
        return 0;
    }
    if(!ctx.field.IsOccupiedAtPos(ctx.target)) {
        return 0;
    }
    int dx = ctx.target.first - ctx.playerPos.first;
    dx = dx*dx;
    int dy = ctx.target.second - ctx.playerPos.second;
    dy = dy*dy;
    int r2 = range * range;
    if(dx+dy > r2) {
        return 0;
    }
    if (ctx.enemies.IsEnemyAtPos(ctx.target)) {
        ctx.enemies.TakeEnemyDamageAtPos(ctx.target, damage);
        const auto& enemies = ctx.enemies.GetEnemies();
        auto it = enemies.find(ctx.target);
        if (it != enemies.end() && it->second.GetHealth() <= 0) {
            ctx.enemies.RemoveEnemyAtPos(ctx.target);
            ctx.field.ChangeisOccupiedAtpos(ctx.target);
            ctx.player.AddPlayerScore(10); 
        }
        return 1;
    }
    if (ctx.buildings.IsBuildingAtPos(ctx.target)) {
        ctx.buildings.TakeBuildingDamageAtPos(ctx.target, damage);
        const auto& buildings = ctx.buildings.GetBuildings();
        auto it = buildings.find(ctx.target);
        if (it != buildings.end() && it->second.GetHealth() <= 0) {
            ctx.buildings.RemoveBuildingAtPos(ctx.target);
            ctx.field.ChangeisOccupiedAtpos(ctx.target);
            ctx.player.AddPlayerScore(20);
        }  
    }
    if (ctx.towerManager.IsTowerAtPos(ctx.target)) {
        ctx.towerManager.TakeTowerDamageAtPos(ctx.target, damage);
        const auto& towers = ctx.towerManager.GetTowers();
        auto it = towers.find(ctx.target);
        if (it != towers.end() && it->second.GetHealth() <= 0) {
            ctx.towerManager.RemoveTowerAtPos(ctx.target);
            ctx.field.ChangeisOccupiedAtpos(ctx.target);
            ctx.player.AddPlayerScore(20);
        }  
    }
    if (ctx.allies.IsAllyAtPos(ctx.target)) {
        ctx.allies.TakeAllyDamageAtPos(ctx.target, damage);
        const auto& allies = ctx.allies.GetAllies();
        auto it = allies.find(ctx.target);
        if (it != allies.end() && it->second.GetHealth() <= 0) {
            ctx.allies.RemoveAllyAtPos(ctx.target);
            ctx.field.ChangeisOccupiedAtpos(ctx.target);
        }  
    }
    return 1;
}

void DirectDamageSpell::enhance() {
    range += 1;
}

std::shared_ptr<dataSpell> DirectDamageSpell::Save(){
    auto data = std::make_shared<directDamageSpell>();
    data->type = SpellType::directDamage;
    data->damage = damage;
    data->range = range;
    return data;
}