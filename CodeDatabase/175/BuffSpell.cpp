#include "BuffSpell.h"
#include <string>

BuffSpell::BuffSpell(BuffType type) : type_(type) {}

std::string BuffSpell::getName() const {
    switch (type_) {
        case BuffType::DAMAGE: return "Buff Damage";
        case BuffType::AREA: return "Buff Area";
        case BuffType::TRAP: return "Buff Trap";
        case BuffType::SUMMON: return "Buff Summon";
        default: return "Buff";
    }
}

bool BuffSpell::apply(Field& field, const Player& player, int targetX, int targetY) {
    return true;
}