#include "TrapSpell.h"
#include "../Controllers/GameState.h"
#include "../LogSystem/Notifier.h"
#include "../LogSystem/SpawnEvent.h"

TrapSpell::TrapSpell(std::pair<int, int> position): damage(DEFAULT_TRAP_DAMAGE), cost(TRAP_SPELL_COST){}

int TrapSpell::getCost() const{
    return this->cost;
}

void TrapSpell::setDMG(int damage){
    this->damage = damage;
}

int TrapSpell::getDMG() const{
    return this->damage;
}

bool TrapSpell::cast(GameState& gameState){
    if(!gameState.getField().canMoveTo(this->position)){
        return false;
    }

    Trap trap = Trap(this->damage);
    trap.setPos(this->position);
    gameState.getTraps().push_back(trap);
    SpawnEvent event = SpawnEvent(this->getName(), "Trap", this->position);
    Notifier::getInstance().notify(event);
    return true;
}

void TrapSpell::upgrade(int power){
    this->damage += power * 10;
}

std::string TrapSpell::getName() const{
    return "Trap spell";
}