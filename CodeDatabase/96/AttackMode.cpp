#include "AttackMode.h"


std::string attackModetoString(AttackMode mode){
    switch (mode) {
        case AttackMode::Melee: return "Melee";
        case AttackMode::Ranged:  return "Ranged";
        default: return "Unknown";
    }
}

AttackMode stringToAttackMode(const std::string& s){
    if (s == "Melee") return AttackMode::Melee;
    if (s == "Range") return AttackMode::Ranged;
}