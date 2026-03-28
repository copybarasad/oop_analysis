#include "DirDMGSpell.h"
#include "../Controllers/GameState.h"
#include "../LogSystem/Notifier.h"
#include "../LogSystem/DamageEvent.h"

DirDMGSpell::DirDMGSpell(int damage, int range, Character& caster, Character& target) : 
            damage(damage), range(range), cost(DIR_SPELL_COST), caster(caster), target(target) {}

DirDMGSpell::DirDMGSpell(Character& caster, Character& target) :
            DirDMGSpell(DEFAULT_DIR_SPELL_DAMAGE, DEFAULT_DIR_SPELL_RANGE, caster, target) {}

int DirDMGSpell::getCost() const{
    return this->cost;
}

void DirDMGSpell::setDMG(int damage){
    this->damage = damage;
}

int DirDMGSpell::getDMG() const{
    return this->damage;
}

void DirDMGSpell::setRange(int range){
    this->range = range;
}

int DirDMGSpell::getRange() const{
    return this->range;
}

bool DirDMGSpell::cast(GameState& gameState){
    if(!caster.isAlive()){
        return false;
    }

    if(caster.squareDistTo(target) <= this->range * this->range){
        DamageEvent event = DamageEvent(this->caster.getName(), this->target.getName(), this->damage);
        Notifier::getInstance().notify(event);
        target.takeDMG(this->damage);
        return true;
    }
    return false;
}

void DirDMGSpell::upgrade(int power){
    this->damage += power * 10;
}

std::string DirDMGSpell::getName() const{
    return "Direct damage spell";
}