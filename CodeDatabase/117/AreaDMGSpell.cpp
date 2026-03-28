#include "AreaDMGSpell.h"
#include "../Controllers/GameState.h"
#include "../LogSystem/Notifier.h"
#include "../LogSystem/DamageEvent.h"

AreaDMGSpell::AreaDMGSpell(Character& caster, std::pair<int, int> pos) : caster(caster), pos(pos){
    this->damage = DEFAULT_AREA_SPELL_DAMAGE;
    this->range = DEFAULT_AREA_SPELL_RANGE;
    this->lenSide = DEFAULT_AREA_SPELL_LENSIDE;
    this->cost = AREA_SPELL_COST;
}

int AreaDMGSpell::getCost() const{
    return this->cost;
}

void AreaDMGSpell::setDMG(int damage){
    this->damage = damage;
}

int AreaDMGSpell::getDMG() const{
    return this->damage;
}

void AreaDMGSpell::setLen(int len){
    this->lenSide = len;
}

int AreaDMGSpell::getLen() const{
    return this->lenSide;
}

void AreaDMGSpell::setRange(int range){
    this->range = range;
}

int AreaDMGSpell::getRange() const{
    return this->range;
}

bool AreaDMGSpell::cast(GameState& gameState){
    if(!this->caster.isAlive()){
        return false;
    }

    if(this->caster.squareDistTo(this->pos) > this->range * this->range){
        return false;
    }

    for(int i = this->pos.first - (this->lenSide - 1) / 2; i < this->pos.first + this->lenSide / 2; ++i){
        for(int j = this->pos.second - (this->lenSide - 1) / 2; j < this->pos.second + this->lenSide / 2; ++j){
            Character* target = gameState.findCharacter(std::make_pair(i, j));
            if(target != nullptr){
                DamageEvent event = DamageEvent(this->caster.getName(), target->getName(), this->damage);
                Notifier::getInstance().notify(event);
                target->takeDMG(this->damage);
            }
        }
    }

    return true;
}

void AreaDMGSpell::upgrade(int power){
    this->lenSide += power;
}

std::string AreaDMGSpell::getName() const{
    return "Area damage spell";
}