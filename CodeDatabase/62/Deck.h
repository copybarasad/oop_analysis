#pragma once

#include <vector>
#include "InstantDamageSpell.h"
#include "HealthSpell.h"
#include "SplashDamageSpell.h"
#include "TrapSpell.h"
#include "BoostSpell.h"
#include "AllySpell.h"

class Deck {
private:
    size_t size;
    std::vector<Spell*> spells;

public:
    Deck(size_t size): size(size), spells() {
        if(size <= 0) {
            std::cerr << "Deck size must be greater than 0" << std::endl;
            exit(1);
        }
        spells.reserve(size);
        SpellType type = static_cast<SpellType>(rand() % 6);
        Spell* spell;
        switch (type) {
            case SpellType::InstantDamage:
                spell = new InstantDamageSpell();
                break;
            case SpellType::Health:
                spell = new HealthSpell();
                break;
            case SpellType::SplashDamage:
                spell = new SplashDamageSpell(rand() % 6 + 1);
                break;
            case SpellType::Trap:
                spell = new TrapSpell(rand() % 11 + 1, rand() % 30 + 1);
                break;
            case SpellType::Boost:
                spell = new BoostSpell();
                break;
            case SpellType::Ally:
                spell = new AllySpell();
                break;
        }
        spells.push_back(spell);
    }

    ~Deck() = default;

    bool isFull() const { return size == spells.size(); }

    size_t getSize() const { return size; }

    size_t getActualSize() const { return spells.size(); }

    void addSpell(Spell* spell) {
        if(spells.size() < size) {
            spells.push_back(spell);
        }
    }

    void removeSpell(const Spell* spell) {
        for(auto it = spells.begin(); it != spells.end(); ++it) {
            if(*it == spell) {
                delete *it;
                spells.erase(it);
                break;
            }
        }
    }

    void removeSpell(size_t index) {
        if(index < 0 || index >= spells.size()) return;
        Spell* spell = spells[index];
        if(spell) {
            delete spell;
            spells.erase(spells.begin() + index);
        }
    }

    Spell* getSpell(size_t index) const {
        if(index < spells.size()) {
            return spells[index];
        }
        return nullptr;
    }

    size_t getIndex(const Spell* spell) const {
        for(size_t i = 0; i < spells.size(); ++i) {
            if(spells[i] == spell) {
                return i;
            }
        }
        return -1;
    }

    std::vector<const Spell*> getSpells() const {
        std::vector<const Spell*> v;
        for(auto it = spells.begin(); it != spells.end(); ++it) {
            v.push_back(*it);
        }
        return v;
    }

    void clear() {
        for(auto it = spells.begin(); it != spells.end(); ++it) {
            if(*it == nullptr) continue;
            delete *it;
        }
        spells.clear();
    }
};