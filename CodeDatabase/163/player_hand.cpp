#include "player_hand.hpp"
#include "direct_damage_spell.hpp"
#include "area_damage_spell.hpp"
#include "game_state.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

bool PlayerHand::randomInitialized = false;

void PlayerHand::initRandom() {
    if (!randomInitialized) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        randomInitialized = true;
    }
}

PlayerHand::PlayerHand(int maxHandSize) 
    : maxHandSize(maxHandSize > 0 ? maxHandSize : 5), 
      enemiesDefeated(0), spellPoints(0) {
    initRandom();
    addSpell(createRandomSpell());
}

bool PlayerHand::addSpell(std::unique_ptr<SpellCard> spell) {
    if (isFull()) {
        return false;
    }
    spells.push_back(std::move(spell));
    return true;
}

bool PlayerHand::removeSpell(int index) {
    if (index < 0 || static_cast<size_t>(index) >= spells.size()) {
        return false;
    }
    spells.erase(spells.begin() + index);
    return true;
}

void PlayerHand::displayHand() const {
    std::cout << "\n=== Рука заклинаний (Ёмкость: " << spells.size() << "/" 
              << maxHandSize << ") ===\n";
    
    for (size_t i = 0; i < spells.size(); ++i) {
        std::cout << i + 1 << ". " << spells[i]->getName() 
                  << " (Дальность: " << spells[i]->getRange() << ")\n";
        std::cout << "   " << spells[i]->getDescription() << "\n";
    }
    
    std::cout << "Очки заклинаний: " << spellPoints << "\n";
    std::cout << "Побеждённых врагов: " << enemiesDefeated << "\n";
}

int PlayerHand::getSpellPoints() const {
    return spellPoints;
}

bool PlayerHand::isFull() const {
    return spells.size() >= static_cast<size_t>(maxHandSize);
}

int PlayerHand::getSize() const {
    return static_cast<int>(spells.size());
}

int PlayerHand::getMaxSize() const {
    return maxHandSize;
}

void PlayerHand::enemyDefeated() {
    enemiesDefeated++;
    spellPoints++;
    
    if (enemiesDefeated % 3 == 0 && !isFull()) {
        std::cout << "\nНовой заклинание за победу " 
                  << enemiesDefeated << " врагов!\n";
        addSpell(createRandomSpell());
    }
}

void PlayerHand::addSpellPoints(int points) {
    if (points > 0) {
        spellPoints += points;
    }
}

bool PlayerHand::canBuySpell() const {
    return spellPoints >= 2 && !isFull();
}

void PlayerHand::buyRandomSpell() {
    if (canBuySpell()) {
        spellPoints -= 2;
        addSpell(createRandomSpell());
        std::cout << "Куплено случайное заклинание. Осталось очков: " << spellPoints << "\n";
    } else {
        if (isFull()) {
            std::cout << "Нет места для нового заклинания.\n";
        } else if (spellPoints < 2) {
            std::cout << "Недостаточно очков заклинаний. Нужно 2, есть " << spellPoints << ".\n";
        }
    }
}

const SpellCard* PlayerHand::getSpell(int index) const {
    if (index >= 0 && static_cast<size_t>(index) < spells.size()) {
        return spells[index].get();
    }
    return nullptr;
}

SpellCard* PlayerHand::getSpell(int index) {
    return const_cast<SpellCard*>(static_cast<const PlayerHand*>(this)->getSpell(index));
}

const std::vector<std::unique_ptr<SpellCard>>& PlayerHand::getSpells() const {
    return spells;
}

std::vector<std::unique_ptr<SpellCard>>& PlayerHand::getSpells() {
    return spells;
}

std::unique_ptr<SpellCard> PlayerHand::createRandomSpell() const {
    initRandom();
    
    if (std::rand() % 2 == 0) {
        std::string names[] = {"Огненная стрела", "Молния", "Магическая стрела"};
        std::string descs[] = {"Прямой снаряд, наносящий урон цели",
                              "Чёрная молния",
                              "Быстрый магический снаряд"};
        int ranges[] = {8, 10, 6};
        int damages[] = {20, 30, 15};
        
        int index = std::rand() % 3;
        return std::make_unique<DirectDamageSpell>(
            names[index], descs[index], ranges[index], damages[index]);
    } else {
        std::string names[] = {"Огненный шар", "Ледяной щар", "Ядовитое облако"};
        std::string descs[] = {"Взрывной снаряд, наносящий урон по области",
                              "Заморозка, которая взрывается",
                              "Токсичный снаряд, создающий ядовитое облако"};
        int ranges[] = {6, 5, 4};
        int damages[] = {15, 20, 10};
        int sizes[] = {2, 2, 3};
        
        int index = std::rand() % 3;
        return std::make_unique<AreaDamageSpell>(
            names[index], descs[index], ranges[index], damages[index], sizes[index]);
    }
}