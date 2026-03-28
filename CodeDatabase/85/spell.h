#ifndef SPELL_H
#define SPELL_H

#include "position.h"
#include <string>

class Game_controller;

class Spell {
protected:
    std::string name;
    std::string description;
    int range;

public:
    Spell(const std::string& spell_name, const std::string& spell_desc, int spell_range);
    virtual ~Spell() = default;

    virtual bool cast(Game_controller& controller, const Position& target) = 0;
    
    const std::string& get_name() const { return name; }
    const std::string& get_description() const { return description; }
    int get_range() const { return range; }
    
    virtual std::string get_type() const = 0;
};

#endif