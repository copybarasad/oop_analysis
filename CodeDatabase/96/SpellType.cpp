#include "SpellType.h"


std::string spellTypetoString(SpellType type){
    switch (type) {
        case SpellType::directDamage: return "directDamage";
        case SpellType::areaDamage:  return "areaDamage";
        case SpellType::trap:  return "trap";
        case SpellType::summAlly:  return "summAlly";
        case SpellType::empower:  return "empower";
        case SpellType::none:  return "none";
        default: return "Unknown";
    }
}

SpellType stringToSpellType(const std::string& s){
    if (s == "empower")     return SpellType::empower;
    if (s == "areaDamage")  return SpellType::areaDamage;
    if (s == "directDamage") return SpellType::directDamage;
    if (s == "summAlly")  return SpellType::summAlly;
    if (s == "trap")        return SpellType::trap;

    //throw std::runtime_error("Unknown SpellType: " + s);
}