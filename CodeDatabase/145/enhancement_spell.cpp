#include "enhancement_spell.h"
#include "enhancement_context.h"
#include <iostream>
#include <sstream>

EnhancementSpell::EnhancementSpell() : enhancementLevel(1) {}

bool EnhancementSpell::apply(const EnhancementContext& context) {
    enhancementLevel++;
    std::cout << "Enhancement spell! Accumulated " << enhancementLevel << " enhancements.\n";
    return true;
}

bool EnhancementSpell::canUse(const EnhancementContext& context) const {
    return true;
}

const char* EnhancementSpell::getName() const {
    return "Enhancement";
}

std::unique_ptr<Spell> EnhancementSpell::clone() const {
    return std::make_unique<EnhancementSpell>();
}

int EnhancementSpell::getEnhancementLevel() const {
    return enhancementLevel;
}

void EnhancementSpell::addEnhancement() {
    enhancementLevel++;
}

void EnhancementSpell::resetEnhancement() {
    enhancementLevel = 0;
}

std::string EnhancementSpell::getSerializedData() const {
    std::ostringstream oss;
    oss << "Enhancement";
    return oss.str();
}