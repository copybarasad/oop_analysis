#include "SummonSpell.h"
#include "../Controllers/GameState.h"
#include "../LogSystem/SpawnEvent.h"
#include "../LogSystem/Notifier.h"

SummonSpell::SummonSpell(): alliesNum(DEFAULT_ALLIES_NUM), cost(SUMMON_SPELL_COST) {}

int SummonSpell::getCost() const{
    return this->cost;
}

void SummonSpell::setAlliesNum(int num){
    this->alliesNum = num;
}

int SummonSpell::getAlliesNum() const{
    return this->alliesNum;
}

bool SummonSpell::cast(GameState& gameState){
    Character& caster = gameState.getPlayer();

    if(!caster.isAlive()){
        return false;
    }

    bool spawned = false;

    for(int i = 0; i < this->alliesNum; ++i){
        std::pair<int, int> spawnPos = gameState.getField().freePosAround(caster.getPos());
        if(spawnPos.first == 0 && spawnPos.second == 0){
            break;
        }

        Ally ally = Ally();
        this->movSys.move(ally, spawnPos, gameState.getField());
        SpawnEvent event = SpawnEvent(this->getName(), "Ally", spawnPos);
        Notifier::getInstance().notify(event);
        gameState.getAllies().push_back(ally);
        spawned = true;
    }

    return spawned;
}

void SummonSpell::upgrade(int power){
    this->alliesNum += power;
}

std::string SummonSpell::getName() const{
    return "Summon spell";
}