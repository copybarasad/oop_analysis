#include "player_hand.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"  
#include "trap_spell.h"
#include "i_levelup_spell.h"
#include "summon_spell.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

PlayerHand::PlayerHand(int maxHandSize, int killsNeeded) 
    : maxSize(maxHandSize), killsForSpell(killsNeeded) {
    std::srand(std::time(0));
}

PlayerHand::~PlayerHand() {
    for (ISpell* spell : spells) {
        delete spell;
    }
}

void PlayerHand::addSpell(ISpell* newSpell) {
    spells.push_back(newSpell);
}

void PlayerHand::addRandomSpell() {
    if (spells.size() >= (unsigned int)maxSize) {
        std::cout << "Рука переполнена!\n";
        return;
    }
    int spellType = std::rand() % MAXSPELLS;  
    
    ISpell* newSpell = nullptr;
    
    switch(spellType) {
        case 0:
            newSpell = new DirectDamageSpell();
            break;
        case 1:
            newSpell = new AreaDamageSpell();  
            break;
        case 2:
            newSpell = new TrapSpell();  
            break;
        case 3:
            newSpell = new SummonSpell(); 
            break;
        case 4:
            newSpell = new ILevelUpSpell(); 
            break;
    }
    spells.push_back(newSpell);
}

void PlayerHand::removeHalfRandomly() {
    if (spells.empty()) return;
    
    size_t targetSize;
    if (spells.size() % 2 == 0) {
        targetSize = spells.size() / 2;
    } else {
        targetSize = (spells.size() + 1) / 2;
    }
    
    while (spells.size() > targetSize) {
        int randomIndex = std::rand() % spells.size();
        delete spells[randomIndex];
        spells.erase(spells.begin() + randomIndex);
    }
}

bool PlayerHand::useSpell(int index, GameMap& map, Position& casterPos, Position& targetPos) {
    if (index < 0 || (unsigned int)index >= spells.size()) {
        return false;
    }
    
    ISpell* spell = spells[index];
    if (spell->cast(map, casterPos, targetPos)) {
        if(spell->getName() != "Улучшение уровня"){
            MapCell& cell = map.getCell(casterPos);
            Entity* entity = cell.getEntity();
            entity->setLevel(entity->getMaxLevel());
        }
        delete spell;
        spells.erase(spells.begin() + index);
        return true;
    }
    
    return false;
}

void PlayerHand::onEnemyKilled(int enemiesKilled)  {
    if (enemiesKilled % killsForSpell == 0) {
        addRandomSpell();
    }
}

int PlayerHand::getSpellCount() const {
    return spells.size();
}

ISpell* PlayerHand::getSpell(int index) const {
    if (index >= 0 && (unsigned int)index < spells.size()) {
        return spells[index];
    }
    return nullptr;
}