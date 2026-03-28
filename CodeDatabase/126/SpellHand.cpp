#include "../../headers/spells/SpellHand.h"
#include "../../headers/spells/DirectDamageSpell.h"
#include "../../headers/spells/AreaDamageSpell.h"
#include "../../headers/spells/TrapSpell.h"
#include "../../headers/exceptions/GameException.h"
#include "../../headers/field/GameField.h"
#include "../../headers/objects/GameObject.h"
#include <chrono>
#include <random>
#include <sstream>
#include <iostream>
#include <map>

using namespace std;

SpellHand::SpellHand(int size) : maxSize(size) {}

void SpellHand::addSpell(shared_ptr<Spell> spell) {
    if (spells.size() < maxSize) {
        spells.push_back(spell);
    }
}

void SpellHand::addRandomSpell() {
    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    mt19937 gen(seed);
    uniform_int_distribution<> dis(0, 2);
    
    shared_ptr<Spell> newSpell;
    switch(dis(gen)) {
        case 0:
            newSpell = make_shared<AreaDamageSpell>();
            break;
        case 1:
            newSpell = make_shared<DirectDamageSpell>();
            break;
        case 2:
            newSpell = make_shared<TrapSpell>();
            break;
    }
    addSpell(newSpell);
}

bool SpellHand::castSpell(int index, const Position& casterPos, const Position& targetPos, GameField& field, vector<shared_ptr<GameObject>>& objects) {
    if (index < 0 || index >= spells.size()) {
        cout << "Invalid spell index!\n";
        return false;
    }
    
    auto& spell = spells[index];
    if (!spell->canCast(casterPos, targetPos, field)) {
        cout << "Cannot cast " << spell->getName() << " at this position!\n";
        return false;
    }
    
    spell->cast(casterPos, targetPos, field, objects);
    return true;
}

void SpellHand::displaySpells() const {
    cout << "=== SPELLS ===\n";
    for (size_t i = 0; i < spells.size(); i++) {
        cout << i << ". " << spells[i]->getName() << " (Range: " << spells[i]->getRange() << ") - " << spells[i]->getDescription() << "\n";
    }
    if (spells.empty()) {
        cout << "No spells available\n";
    }
    cout << "==============\n";
}

size_t SpellHand::getSpellCount() const { return spells.size(); }

int SpellHand::getMaxSize() const { return maxSize; }

void SpellHand::removeRandomSpells(int count) {
    if (count >= static_cast<int>(spells.size())) {
        spells.clear();
        return;
    }
    
    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    mt19937 gen(seed);
    
    for (int i = 0; i < count; i++) {
        if (spells.empty()) break;
        uniform_int_distribution<> dis(0, spells.size() - 1);
        int index = dis(gen);
        spells.erase(spells.begin() + index);
    }
}

string SpellHand::serialize() const {
    stringstream ss;
    ss << spells.size() << "\n";
    for (const auto& spell : spells) {
        ss << spell->serialize() << "\n";
    }
    return ss.str();
}

void SpellHand::deserialize(istream& stream) {
    spells.clear();
    int spellCount;
    stream >> spellCount;
    if (stream.fail()) throw CorruptedSaveException("", "Invalid spell count");
    
    string line;
    getline(stream, line);
    
    map<string, function<shared_ptr<Spell>(const string&)>> spellFactories = {
        {"DirectDamage", [](const string& data) {
            stringstream ss(data);
            string type;
            int damage, range;
            string name;
            if(!(ss >> type >> damage >> range >> name)){
                throw invalid_argument("Invalid spell data format");
            }
            return make_shared<DirectDamageSpell>(damage, range, name);
        }},
        {"AreaDamage", [](const string& data) {
            stringstream ss(data);
            string type;
            int damage, range;
            string name;
            if(!(ss >> type >> damage >> range >> name)){
                throw invalid_argument("Invalid spell data format");
            }
            return make_shared<AreaDamageSpell>(damage, range, name);
        }},
        {"TrapSpell", [](const string& data) {
            stringstream ss(data);
            string type;
            int damage, range;
            string name;
            if(!(ss >> type >> damage >> range >> name)){
                throw invalid_argument("Invalid spell data format");
            }
            return make_shared<TrapSpell>(damage, range, name);
        }}
    };
    
    for (int i = 0; i < spellCount; i++) {
        getline(stream, line);
        stringstream ss(line);
        string spellType;
        ss >> spellType;
        
        if (spellFactories.find(spellType) != spellFactories.end()) {
            spells.push_back(spellFactories[spellType](line));
        }
    }
}