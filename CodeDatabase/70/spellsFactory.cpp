#include "spellsFactory.h"

SpellsFactory& SpellsFactory::getInstance()
{
    static SpellsFactory instance;
    return instance;
}

Spell* SpellsFactory::createSpell(const std::string& type, std::istream& is) 
{
    auto it = creators.find(type);
    if (it != creators.end()) {
        return it->second(is);
    }
    return nullptr;
}