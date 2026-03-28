#include <vector>
#include <utility>
#include <memory>
#include <cmath>
#include "AreaDamageSpell.h"
#include "EnemyManager.h"
#include "BuildingManager.h"
#include "TowerManager.h"
#include "PlayerManager.h"
#include "AllyManager.h"


bool AreaDamageSpell::Cast(SpellContext& ctx) {
    bool result = 0;
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
    int r2 = radius*radius;
    if(dx+dy > r2) {
        return 0;
    }

    for(int x = ctx.target.first - radius; x <= ctx.target.first + radius; ++x) {
        for(int y = ctx.target.second - radius; y <= ctx.target.second + radius; ++y) {
            if(ctx.field.inBounds({x, y})) {

                if(ctx.field.IsOccupiedAtPos({x, y}) && !(x == ctx.playerPos.first && y == ctx.playerPos.second)) {
                    if (ctx.enemies.IsEnemyAtPos({x, y})) {
                        ctx.enemies.TakeEnemyDamageAtPos({x, y}, damage);
                        const auto& enemies = ctx.enemies.GetEnemies();
                        auto it = enemies.find({x, y});
                        if (it != enemies.end() && it->second.GetHealth() <= 0) {
                            ctx.enemies.RemoveEnemyAtPos({x, y});
                            ctx.field.ChangeisOccupiedAtpos({x, y});
                            ctx.player.AddPlayerScore(10); 
                        }
                    }
                    if (ctx.buildings.IsBuildingAtPos({x, y})) {
                        ctx.buildings.TakeBuildingDamageAtPos({x, y}, damage);
                        const auto& buildings = ctx.buildings.GetBuildings();
                        auto it = buildings.find({x, y});
                        if (it != buildings.end() && it->second.GetHealth() <= 0) {
                            ctx.buildings.RemoveBuildingAtPos({x, y});
                            ctx.field.ChangeisOccupiedAtpos({x, y});
                            ctx.player.AddPlayerScore(20);
                        }  
                    }
                    if (ctx.towerManager.IsTowerAtPos({x, y})) {
                        ctx.towerManager.TakeTowerDamageAtPos({x, y}, damage);
                        const auto& towers = ctx.towerManager.GetTowers();
                        auto it = towers.find({x, y});
                        if (it != towers.end() && it->second.GetHealth() <= 0) {
                            ctx.towerManager.RemoveTowerAtPos({x, y});
                            ctx.field.ChangeisOccupiedAtpos({x, y});
                            ctx.player.AddPlayerScore(20);
                        }
                    }
                    if (ctx.allies.IsAllyAtPos({x, y})) {
                        ctx.allies.TakeAllyDamageAtPos({x, y}, damage);
                        const auto& allies = ctx.allies.GetAllies();
                        auto it = allies.find({x, y});
                        if (it != allies.end() && it->second.GetHealth() <= 0) {
                            ctx.allies.RemoveAllyAtPos({x, y});
                            ctx.field.ChangeisOccupiedAtpos({x, y});
                        }
                    }
                    result = 1;
                    
                }
            }
        }
    }
    return result;
}

void AreaDamageSpell::enhance() {
    radius += 1;
}

std::shared_ptr<dataSpell> AreaDamageSpell::Save(){
    auto data = std::make_shared<areaDamageSpell>();
    data->type = SpellType::areaDamage;
    data->damage=damage;
    data->radius=radius;
    return data;
} 