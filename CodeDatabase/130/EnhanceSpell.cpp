#include "EnhanceSpell.h"
#include <iostream>

EnhanceSpell::EnhanceSpell(const std::string& name)
    : name_(name) {
    bonus_.rangeBonus = 1;
    bonus_.areaBonus = 1;
    bonus_.damageBonus = 5;
    bonus_.summonBonus = 1;
    bonus_.enhanceStack = 1;
}

bool EnhanceSpell::cast(SpellContext& context, EnhancementState& enhancements) {
    enhancements.rangeBonus += bonus_.rangeBonus;
    enhancements.areaBonus += bonus_.areaBonus;
    enhancements.damageBonus += bonus_.damageBonus;
    enhancements.summonBonus += bonus_.summonBonus;
    enhancements.enhanceStack += bonus_.enhanceStack;
    
    std::cout << "Cast " << name_ << "! Next spell will be enhanced!\n";
    std::cout << "Enhancement stack: " << enhancements.enhanceStack << "\n";

    return true;
}

std::string EnhanceSpell::getName() const {
    return name_;
}

std::string EnhanceSpell::getDescription() const {
    return name_ + " (Enhances next spell)";
}

SpellType EnhanceSpell::getType() const {
    return SpellType::ENHANCE;
}

std::unique_ptr<Spell> EnhanceSpell::clone() const {
    auto spell = std::make_unique<EnhanceSpell>(name_);
    EnhancementState delta;
    delta.rangeBonus = bonus_.rangeBonus - 1;
    delta.areaBonus = bonus_.areaBonus - 1;
    delta.damageBonus = bonus_.damageBonus - 5;
    delta.summonBonus = bonus_.summonBonus - 1;
    delta.enhanceStack = bonus_.enhanceStack - 1;
    
    if (delta.rangeBonus > 0 || delta.areaBonus > 0 || delta.damageBonus > 0 ||
        delta.summonBonus > 0 || delta.enhanceStack > 0) {
        spell->applyPermanentBonus(delta);
    }
    return spell;
}

void EnhanceSpell::applyPermanentBonus(const EnhancementState& bonus) {
    bonus_.rangeBonus += bonus.rangeBonus;
    bonus_.areaBonus += bonus.areaBonus;
    bonus_.damageBonus += bonus.damageBonus;
    bonus_.summonBonus += bonus.summonBonus;
    bonus_.enhanceStack += bonus.enhanceStack;
    if (bonus_.enhanceStack < 1) bonus_.enhanceStack = 1;
}

