#include "EnhanceSpell.h"
#include "ISpellContext.h"
#include <ostream>

EnhanceSpell::EnhanceSpell(Enhancement enh)
    : enhancement(enh),
      name("Enhance"),
      description("Enhances next spell") {
}

bool EnhanceSpell::cast(ISpellContext &context) {
    context.addEnhancement(enhancement);
    context.addMessage("Enhancement applied! Next spell will be boosted.");
    return true;
}

bool EnhanceSpell::castAt(ISpellContext &context, int, int) {
    return cast(context);
}

std::string EnhanceSpell::getName() const {
    return name;
}

std::string EnhanceSpell::getDescription() const {
    return description;
}

SpellType EnhanceSpell::getType() const {
    return SpellType::ENHANCE;
}

Enhancement EnhanceSpell::getEnhancement() const {
    return enhancement;
}

Spell *EnhanceSpell::clone() const {
    return new EnhanceSpell(*this);
}

Spell *EnhanceSpell::upgradedCopy() const {
    Enhancement upgraded = enhancement;
    upgraded.value += 1;
    return new EnhanceSpell(upgraded);
}

bool EnhanceSpell::requiresTarget() const {
    return false;
}

bool EnhanceSpell::enhance(const Enhancement &) {
    return false;
}

void EnhanceSpell::serialize(std::ostream &out) const {
    out << static_cast<int>(enhancement.type) << " " << enhancement.value;
}
