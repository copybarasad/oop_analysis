#include "spell.hpp"

Spell::Spell(const std::string& name,const std::string& disc) : name(name),disc(disc){}

std::string Spell::get_name()const{
    return name;
}

std::string Spell::get_disc()const{
    return disc;
}
