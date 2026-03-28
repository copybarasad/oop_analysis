#ifndef SPELLSFACTORY_H
#define SPELLSFACTORY_H

#include "spell.h"
#include <memory>
#include <functional>
#include <unordered_map>

class SpellsFactory {
private:
    std::unordered_map<std::string, std::function<Spell*(std::istream&)>> creators;

public:
    static SpellsFactory& getInstance();

    template<typename T>
    void registerSpell(const std::string& type) 
    {
        creators[type] = [](std::istream& is) -> Spell* {
            auto spell = new T();
            spell->deserialize(is);
            return spell;
        };
    }
    
    Spell* createSpell(const std::string& type, std::istream& is);
};

#endif