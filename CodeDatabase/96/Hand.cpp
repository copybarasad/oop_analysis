#include <vector>
#include <memory>
#include <random>
#include "Hand.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummAllySpell.h"
#include "EmpowerSpell.h"
#include "SpellContext.h"


#include <iostream>




Hand::Hand() : maxSpell(3), currentSpell(0) {
    AddRandomSpell();
}

Hand::Hand(dataHand data):maxSpell(data.maxSpell), currentSpell(data.currentSpell){
    for(auto s:data.spells){
        if (auto empower = dynamic_cast<empowerSpell*>(s.get())) {
            auto spell = std::make_shared<EmpowerSpell>(empower->bonus);
            spells.push_back(spell);
        } else if(auto areadamage = dynamic_cast<areaDamageSpell*>(s.get())){
            auto spell = std::make_shared<AreaDamageSpell>(areadamage->damage,areadamage->radius);
            spells.push_back(spell);
        } else if(auto directdamage = dynamic_cast<directDamageSpell*>(s.get())){
            auto spell = std::make_shared<DirectDamageSpell>(directdamage->damage,directdamage->range);
            spells.push_back(spell);
        } else if(auto summally = dynamic_cast<summAllySpell*>(s.get())){
            auto spell = std::make_shared<SummAllySpell>(summally->numAllies);
            spells.push_back(spell);
        } else if(auto trap = dynamic_cast<trapSpell*>(s.get())){
            auto spell = std::make_shared<TrapSpell>(trap->damage, trap->range);
            spells.push_back(spell);
        }
    }

}



bool Hand::AddSpell(std::shared_ptr<Spell> spell) {
    if (spells.size() < maxSpell) {
        spells.push_back(spell);
        return true;
    }
    return false;
}



bool Hand::AddRandomSpell() {
    if (spells.size() >= maxSpell) {
        return false;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    // Список допустимых типов
    std::vector<SpellType> availableTypes = {
        SpellType::directDamage,
        SpellType::areaDamage,
        SpellType::trap,
        SpellType::summAlly,
        SpellType::empower
    };

    std::uniform_int_distribution<size_t> dist(0, availableTypes.size() - 1);

    SpellType randomType = availableTypes[dist(gen)];

    std::shared_ptr<Spell> newSpell;

    switch (randomType) {
        case SpellType::directDamage:
            newSpell = std::make_shared<DirectDamageSpell>();
            break;
        case SpellType::areaDamage:
            newSpell = std::make_shared<AreaDamageSpell>();
            break;
        case SpellType::trap:
            newSpell = std::make_shared<TrapSpell>();
            break;
        case SpellType::summAlly:
            newSpell = std::make_shared<SummAllySpell>();
            break;
        case SpellType::empower:
            newSpell = std::make_shared<EmpowerSpell>();
            break;
        default:
            return false;
    }
    spells.push_back(newSpell);
    return true;
}


bool Hand::UseSpell(SpellContext& ctx) {
    if (spells.empty() || currentSpell < 0 || currentSpell >= spells.size()) {
        return 0;
    }
    if( dynamic_cast<EmpowerSpell*>(spells[currentSpell].get())) {
        if(spells.size() == 1 || ctx.target.first == currentSpell){
            return 0;
        }
        ctx.spell = spells[ctx.target.first];
    }
    bool cast = spells[currentSpell]->Cast(ctx);
    if(!cast) {
        return 0;
    }
    
    //delete spells[currentSpell].get();
    spells.erase(spells.begin() + currentSpell);
    
    currentSpell = 0;
    return 1;
}

void Hand::SellectSpell(int index) {
    if (index >= 0 && index < spells.size()) {
        currentSpell = index;
    }
}

const SpellType Hand::GetCurrentSpellType() const {
    if (spells.empty() || currentSpell < 0 || currentSpell >= spells.size()) {
        return SpellType::none;
    }

    if (dynamic_cast<DirectDamageSpell*>(spells[currentSpell].get())) {
        return SpellType::directDamage;
    } else if (dynamic_cast<AreaDamageSpell*>(spells[currentSpell].get())) {
        return SpellType::areaDamage;
    } else if (dynamic_cast<TrapSpell*>(spells[currentSpell].get())) {
        return SpellType::trap;
    } else if (dynamic_cast<SummAllySpell*>(spells[currentSpell].get())) {
        return SpellType::summAlly;
    } else if (dynamic_cast<EmpowerSpell*>(spells[currentSpell].get())) {
        return SpellType::empower;
    } else {
        return SpellType::none;
    }
}

std::shared_ptr<Spell> Hand::GetCurrentSpell(){
    return spells[currentSpell];
}

dataHand Hand::Save(){
    dataHand data;
    data.maxSpell=maxSpell;
    data.currentSpell=currentSpell;
    for(auto& spell : spells){
        std::shared_ptr<dataSpell> datae = spell->Save();
        data.spells.push_back(datae);
    }
    return data;
}

void Hand::ImproveAllSpells(){
    for(auto s:spells){
        s->enhance();
    }
}