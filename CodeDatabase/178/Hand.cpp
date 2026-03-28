#include "Hand.h"
#include "Player.h"
#include "SpellSystem.h"  
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "UpgradeSpell.h"
#include <iostream>
#include <string>

Hand::Hand(int maxSize) : maxSize(maxSize) {}

void Hand::addSpell(std::unique_ptr<ISpell> spell) {
    if ((int)spells.size() < maxSize) spells.push_back(std::move(spell));
}

bool Hand::buySpell(std::unique_ptr<ISpell> spell, Player &player, int cost) {
    if ((int)spells.size() >= maxSize) { std::cout<<"Hand is full.\n"; return false; }
    if (!player.trySpendCoins(cost)) { std::cout<<"Not enough coins to buy spell.\n"; return false; }
    spells.push_back(std::move(spell));
    return true;
}

void Hand::removeAt(int index) {
    if (index<0 || index>=(int)spells.size()) return;
    spells.erase(spells.begin()+index);
}

int Hand::size() const { return (int)spells.size(); }

const ISpell* Hand::getAt(int index) const {
    if (index<0 || index>=(int)spells.size()) return nullptr;
    return spells[index].get();
}

std::unique_ptr<ISpell> Hand::extractAt(int index) {
    if (index<0 || index>=(int)spells.size()) return nullptr;
    auto ptr = std::move(spells[index]);
    spells.erase(spells.begin()+index);
    return ptr;
}

void Hand::list() const {
    std::cout << "Hand ("<<spells.size()<<"/"<<maxSize<<"):\n";
    for (int i=0;i<(int)spells.size();++i) {
        std::cout << i << ": " << spells[i]->name() << "\n";
    }
}

void Hand::listSpells(const Player &player) const {
    std::cout << "=== Player Hand ===" << std::endl;
    std::cout << "Coins: " << player.getCoins() << std::endl;
    std::cout << "Upgrade cost: 25 coins" << std::endl;
    
    if (spells.empty()) {
        std::cout << "Your hand is empty." << std::endl;
        std::cout << "====================" << std::endl;
        return;
    }
    
    for (size_t i = 0; i < spells.size(); ++i) {
        const ISpell* spell = spells[i].get();
        std::cout << i << ") " << spell->name();
        if (spell->isImproved()) {
            std::cout << " - improved!";
        }
        
        std::cout << std::endl;
    }
    std::cout << "====================" << std::endl;
}

bool Hand::upgradeSpell(int index, int upgradePower) {
    if (index < 0 || index >= (int)spells.size()) {
        std::cout << "Invalid spell index: " << index << std::endl;
        return false;
    }
    
    ISpell* spell = spells[index].get();
    const char* spellName = spell->name();
    bool upgraded = false;
    std::cout << "Upgrading spell: " << spellName << " at index " << index << std::endl;
    
    if (std::string(spellName) == "Direct Damage") {
        DirectDamageSpell* directSpell = dynamic_cast<DirectDamageSpell*>(spell);
        if (directSpell) {
            directSpell->increaseDamage(upgradePower * 5);
            directSpell->increaseRange(1);
            upgraded = true;
        }
    }
    else if (std::string(spellName) == "Area Damage") {
        AreaDamageSpell* areaSpell = dynamic_cast<AreaDamageSpell*>(spell);
        if (areaSpell) {
            areaSpell->increaseDamage(upgradePower * 3);
            areaSpell->increaseArea(1, 1);
            upgraded = true;
        }
    }
    else if (std::string(spellName) == "Trap") {
        TrapSpell* trapSpell = dynamic_cast<TrapSpell*>(spell);
        if (trapSpell) {
            trapSpell->increaseDamage(upgradePower * 4);
            upgraded = true;
        }
    }
    else if (std::string(spellName) == "Summon") {
        SummonSpell* summonSpell = dynamic_cast<SummonSpell*>(spell);
        if (summonSpell) {
            summonSpell->increaseCount(upgradePower);
            upgraded = true;
        }
    }
    if (upgraded) {
        spell->setImproved(true);
    }
    
    return upgraded;
}

const ISpell* Hand::getSpellInfo(int index) const {
    if (index < 0 || index >= (int)spells.size()) {
        return nullptr;
    }
    return spells[index].get();
}