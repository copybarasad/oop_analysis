#include "Hand.h"
#include <iostream>
#include <cstdlib>
#include "DamageSpell.h"
#include "SplashSpell.h"
#include "TrapSpell.h"
#include "Logger.h"
#include <random>
#include <algorithm>

Hand::Hand(int size)
    : size(size) {
}

void Hand::addSpell(std::shared_ptr<Spell> spell) {
    if ((int)spells.size() < size) {
        spells.push_back(std::move(spell));
    }
}

std::shared_ptr<Spell> Hand::getSpell(int index) const {
    if (index < 0 || index >= (int)spells.size()) return nullptr;
    return spells[index];
}

void Hand::useSpell(int index) {
    if (index >= 0 && index < (int)spells.size()) {
        spells.erase(spells.begin() + index);
    }
}

int Hand::getCount() const {
    return (int)spells.size();
}

std::shared_ptr<Spell> Hand::generateSpell() {
    int typeRand = rand() % 3;  // 0 - Damage, 1 - Splash, 2 - Trap
    int dmg = 1 + rand() % 10;  // 1�10
    int dist = 2 + rand() % 4;  // 2�5
    int radius = 2;
    std::shared_ptr<Spell> spell;

    switch (typeRand) {
    case 0:
        spell = std::make_shared<DamageSpell>(dmg, dist);
        break;
    case 1:
        spell = std::make_shared<SplashSpell>(dmg, dist, radius);
        break;
    case 2:
        spell = std::make_shared<TrapSpell>(dmg, Coords{ 0, 0 });
        break;
    }

    addSpell(spell);
    return spell;
}

void Hand::log() const {
    Logger::log() << "Hand state: size = " << size << ". Spells:\n";
    for (const auto& spell: spells) 
        Logger::log() << "Spell:" << spell->getType() << ", damage:" << spell->getDamage() << "\n";
}

json Hand::serialize() const {
    json j;
    j["size"] = size;
    j["spells"] = json::array();

    for (const auto& spell : spells) {
        if (spell) {
            j["spells"].push_back(spell->serialize());
        }
    }

    return j;
}

void Hand::deserialize(const json& j) {
    size = j.at("size").get<int>();
    spells.clear();

    for (const auto& spellJson : j.at("spells")) {
        std::string type = spellJson.at("type").get<std::string>();
        std::shared_ptr<Spell> spell;

        if (type == "Damage") spell = std::make_shared<DamageSpell>(-1, -1);
        else if (type == "Splash") spell = std::make_shared<SplashSpell>(-1, -1, -1);
        else if (type == "Trap") spell = std::make_shared<TrapSpell>(-1);

        if (spell) {
            spell->deserialize(spellJson);
            spells.push_back(spell);
        }
    }
}

void Hand::removeHalfRandomSpells() {
    if (spells.empty()) return;

    size_t numToRemove = spells.size() / 2;

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(spells.begin(), spells.end(), g);

    spells.erase(spells.begin(), spells.begin() + numToRemove);
}