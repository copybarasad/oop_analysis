#include "SpellHand.h"
#include "GameState.h"
#include "Player.h"
#include "SpellFactory.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include <iostream>
#include <random>

SpellHand::SpellHand(size_t handSize) : maxSize(handSize) {}

SpellHand::SpellHand(const SpellHand& other) 
    : maxSize(other.maxSize), 
      enemiesDefeated(other.enemiesDefeated),
      enemiesForNewSpell(other.enemiesForNewSpell) {
    
    for (const auto& spell : other.spells) {
        if (spell) {
            if (auto directSpell = dynamic_cast<const DirectDamageSpell*>(spell.get())) {
                spells.push_back(std::make_unique<DirectDamageSpell>(
                    directSpell->getName(), 
                    directSpell->getDamage(),
                    directSpell->getManaCost(),
                    directSpell->getRange()
                ));
            } else if (auto areaSpell = dynamic_cast<const AreaDamageSpell*>(spell.get())) {
                spells.push_back(std::make_unique<AreaDamageSpell>(
                    areaSpell->getName(),
                    areaSpell->getDamage(),
                    areaSpell->getManaCost(),
                    areaSpell->getRange(),
                    areaSpell->getAreaSize()
                ));
            }
        }
    }
}

SpellHand& SpellHand::operator=(const SpellHand& other) {
    if (this != &other) {
        maxSize = other.maxSize;
        enemiesDefeated = other.enemiesDefeated;
        enemiesForNewSpell = other.enemiesForNewSpell;
        
        spells.clear();
        
        for (const auto& spell : other.spells) {
            if (spell) {
                if (auto directSpell = dynamic_cast<const DirectDamageSpell*>(spell.get())) {
                    spells.push_back(std::make_unique<DirectDamageSpell>(
                        directSpell->getName(), 
                        directSpell->getDamage(),
                        directSpell->getManaCost(),
                        directSpell->getRange()
                    ));
                } else if (auto areaSpell = dynamic_cast<const AreaDamageSpell*>(spell.get())) {
                    spells.push_back(std::make_unique<AreaDamageSpell>(
                        areaSpell->getName(),
                        areaSpell->getDamage(),
                        areaSpell->getManaCost(),
                        areaSpell->getRange(),
                        areaSpell->getAreaSize()
                    ));
                }
            }
        }
    }
    return *this;
}

bool SpellHand::addSpell(std::unique_ptr<Spell> spell) {
    if (spells.size() >= maxSize) {
        return false;
    }
    spells.push_back(std::move(spell));
    return true;
}

bool SpellHand::castSpell(size_t index, const Position& target, GameState& gameState, const Player& caster) {
    if (index >= spells.size()) {
        return false;
    }

    return spells[index]->cast(target, gameState, caster);
}

void SpellHand::removeSpell(size_t index) {
    if (index < spells.size()) {
        spells.erase(spells.begin() + index);
    }
}

const Spell* SpellHand::getSpell(size_t index) const {
    return (index < spells.size()) ? spells[index].get() : nullptr;
}

void SpellHand::onEnemyDefeated() {
    enemiesDefeated++;
    std::cout << "Побежден враг! Прогресс: " << enemiesDefeated << "/" << enemiesForNewSpell << std::endl;
    

    if (tryLearnNewSpell()) {
        std::cout << "Получено новое заклинание автоматически!" << std::endl;
    }
}

bool SpellHand::canGetNewSpell() const {
    return enemiesDefeated >= enemiesForNewSpell && spells.size() < maxSize;
}

bool SpellHand::tryLearnNewSpell() {
    if (canGetNewSpell()) {
        auto newSpell = SpellFactory::createRandomSpell();
        if (addSpell(std::move(newSpell))) {
            resetEnemyCounter();
            return true;
        }
    }
    return false;
}

void SpellHand::resetEnemyCounter() {
    enemiesDefeated = 0;
}

void SpellHand::displaySpells() const {
    std::cout << "Заклинания в руке (" << spells.size() << "/" << maxSize << "):" << std::endl;
    for (size_t i = 0; i < spells.size(); ++i) {
        const Spell* spell = spells[i].get();
        if (spell) {
            std::cout << i + 1 << ". " << spell->getName() 
                      << " (Урон: " << spell->getDamage()       
                      << ", Мана: " << spell->getManaCost() 
                      << ", Дальность: " << spell->getRange() << ")" << std::endl;
        }
    }

    if (spells.size() < maxSize) {
        std::cout << "Прогресс до следующего заклинания: " << enemiesDefeated << "/" << enemiesForNewSpell << std::endl;
    } else {
        std::cout << "Рука заклинаний заполнена!" << std::endl;
    }
}