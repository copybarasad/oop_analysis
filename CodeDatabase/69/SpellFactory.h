#pragma once
#include <string>
#include <memory>

#include "Spell.h"

struct SpellFactory 
{
    static std::unique_ptr<Spell> CreateByName(const std::string& name);
};