#include "BehaviourSystem.h"
#include <memory>
#include "../LogSystem/Notifier.h"
#include "../LogSystem/DamageEvent.h"
#include "../LogSystem/TrapTriggerEvent.h"
#include "../LogSystem/DeathEvent.h"
#include "../LogSystem/SpawnEvent.h"
#include "../LogSystem/PlayerGainEXPEvent.h"
#include "../LogSystem/SpellCastEvent.h"

BehaviourSystem::BehaviourSystem(GameState& gameState): gameState(gameState) {}

void BehaviourSystem::enemyTurn(Enemy& enemy){
    Player& player = this->gameState.getPlayer();

    if(!enemy.isAlive()){
        return;
    }

    if (enemy.squareDistTo(player) <= 2){
        player.takeDMG(enemy.getDMG());
        DamageEvent event = DamageEvent("Enemy", "player", enemy.getDMG());
        Notifier::getInstance().notify(event);
    }
    else{
        this->movSys.stepToTar(enemy, player, gameState.getField());
    }
}

void BehaviourSystem::allyTurn(Ally& ally){
    if(!ally.isAlive()){
        return;
    }

    std::vector<Enemy>& enemies = this->gameState.getEnemies();
    std::vector<EnemyBuilding>& builds = this->gameState.getBuilds();
    std::vector<EnemyTower>& towers = this->gameState.getTowers();

    int minSquareDist = INF;
    Character* nearest = nullptr;
    for(int i = 0; i < enemies.size(); ++i){
        int squareDist = ally.squareDistTo(enemies[i]);
        if(squareDist <= minSquareDist){
            minSquareDist = squareDist;
            nearest = &(enemies[i]);
        }
    }
    for(int i = 0; i < builds.size(); ++i){
        int squareDist = ally.squareDistTo(builds[i]);
        if(squareDist <= minSquareDist){
            minSquareDist = squareDist;
            nearest = &(builds[i]);
        }
    }
    for(int i = 0; i < towers.size(); ++i){
        int squareDist = ally.squareDistTo(towers[i]);
        if(squareDist <= minSquareDist){
            minSquareDist = squareDist;
            nearest = &(towers[i]);
        }
    }
    
    if(nearest == nullptr){
        return;
    }

    if(!nearest->isAlive()){
        return;
    }

    if(ally.squareDistTo(*nearest) <= 2){
        nearest->takeDMG(ally.getDMG());
        DamageEvent DMGevent = DamageEvent("Ally", nearest->getName(), ally.getDMG());
        Notifier::getInstance().notify(DMGevent);
        if(!nearest->isAlive()){
            this->gameState.getPlayer().addEXP(EXP_FOR_KILL_BY_ALLY);
            PlayerGainEXPEvent event = PlayerGainEXPEvent(EXP_FOR_KILL);
            Notifier::getInstance().notify(event);
        }
    }
    else{
        this->movSys.stepToTar(ally, *nearest, this->gameState.getField());
    }
}

void BehaviourSystem::alliesTurn(){
    std::vector<Ally>& allies = this->gameState.getAllies();
    for(int i = 0; i < allies.size(); ++i){
        this->allyTurn(allies[i]);
    }
    this->gameState.deleteDead();
}

void BehaviourSystem::spawnEnemy(EnemyBuilding& build){
    if(!build.isAlive()){
        return;
    }

    std::pair<int, int> buildPos = build.getPos();
    std::pair<int, int> posToSpawn = this->gameState.getField().freePosAround(buildPos);
    if(posToSpawn.first == 0 && posToSpawn.second == 0){
        return;
    }

    Enemy enemy = Enemy(build.getEnemyHP(), build.getEnemyDMG(), CharacterCondition::Normal);
    movSys.move(enemy, posToSpawn, this->gameState.getField());
    this->gameState.getEnemies().push_back(enemy);
    SpawnEvent event = SpawnEvent("Enemy building", "enemy", posToSpawn);
    Notifier::getInstance().notify(event);
}

void BehaviourSystem::updateBuild(EnemyBuilding& build){
    if(!build.isAlive()){
        return;
    }

    build.updateCounter();
    if(build.getTurns() > 0){
        return;
    }

    this->spawnEnemy(build);
}

void BehaviourSystem::enemiesTurn(){
    std::vector<Enemy>& enemies = this->gameState.getEnemies();
    if(!enemies.empty()){
        for(int i = 0; i < enemies.size(); ++i){
            this->enemyTurn(enemies[i]);
        }
    }
    this->gameState.deleteDead();
}

void BehaviourSystem::buildingsTurn(){
    std::vector<EnemyBuilding>& builds = this->gameState.getBuilds();
    if(!builds.empty()){
        for(int i = 0; i < builds.size(); ++i){
            this->updateBuild(builds[i]);
        }
    }
    this->gameState.deleteDead();
    
    std::vector<EnemyTower>& towers = this->gameState.getTowers();
    for(int i = 0; i < towers.size(); ++i){
        std::unique_ptr<DirDMGSpell> spell = std::make_unique<DirDMGSpell>(towers[i], gameState.getPlayer());
        if (spell->cast(gameState)){
            SpellCastEvent event = SpellCastEvent(spell->getName(), "Enemy Tower");
            Notifier::getInstance().notify(event);
        }
    }
    this->gameState.deleteDead();
}

void BehaviourSystem::updateTraps(){
    std::vector<Trap>& traps = this->gameState.getTraps();
    for(int i = 0; i < traps.size(); ++i){
        int index = this->gameState.findEnemy(traps[i].getPos());
        if(index >= 0){
            traps[i].activate(this->gameState.getEnemies()[index]);
            TrapTriggerEvent event = TrapTriggerEvent("Enemy", traps[i].getDMG(), traps[i].getPos());
            Notifier::getInstance().notify(event);
        }
    }
    this->gameState.deleteDead();
}
