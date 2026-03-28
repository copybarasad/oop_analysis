#include "hand.h"
#include "event_system.h"

Hand::Hand(size_t maxSize): maxSize_(maxSize) {}

Hand::~Hand() {
    
    for (ISpell* spell : spells_) {
        delete spell;
    }
    spells_.clear();
}

bool Hand::addSpell(ISpell* spell) {
    if (spells_.size() >= maxSize_ || !spell) return false;
    spells_.push_back(spell);
    return true;
}

bool Hand::playSpell(size_t index, int casterId, Field& field, int targetRow, int targetCol) {
    if (index >= spells_.size()) return false;
    std::string spellName = spells_[index]->getName();
    bool used = spells_[index]->apply(casterId, field, targetRow, targetCol);
    if (used) { 
        EventSystem::getInstance().logSpellCasted(spellName);
        delete spells_[index];
        spells_.erase(spells_.begin() + index);
    }
    return used;
}

void Hand::drawSpell(ISpell* spell) {
    if (spells_.size() >= maxSize_ || !spell) return;
    spells_.push_back(spell);
}

size_t Hand::size() const { return spells_.size(); }
size_t Hand::capacity() const { return maxSize_; }

std::string Hand::getSpellName(size_t index) const {
    if (index >= spells_.size()) return std::string();
    return spells_[index]->getName();
}
#include "direct_damage_spell.h"
#include "area_spell.h"
#include "trap_spell.h"
#include "summon_spell.h"
#include "buff_spell.h"
#include <random>
#include <iostream>

void Hand::addRandomSpell() {
    if (spells_.size() >= maxSize_) return;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 4);
    
    ISpell* sp = nullptr;
    int t = dist(gen);
    
 
    if (t == 0) {
        sp = new DirectDamageSpell(8, 3);
        std::cout << "Created DirectDamageSpell" << std::endl;
    }
    else if (t == 1) {
        sp = new AreaDamageSpell(6, 2);
        std::cout << "Created AreaDamageSpell" << std::endl;
    }
    else if (t == 2) {
        sp = new TrapSpell(6, 2);
        std::cout << "Created TrapSpell" << std::endl;
    }
    else if (t == 3) {
        sp = new SummonSpell(6, 2, 1, 2);
        std::cout << "Created SummonSpell" << std::endl;
    }
    else {
        sp = new BuffSpell(1, 1, 2, 1);
        std::cout << "Created BuffSpell" << std::endl;
    }
    
    if (sp) {
        drawSpell(sp);
        EventSystem::getInstance().logSpellLearned(sp->getName());
    }
}
