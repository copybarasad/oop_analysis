#include "EnhancementSpell.h"
#include "Game.h"
#include <mutex>
#include <string>

static Enhancement pendingEnhancement;
static std::mutex enhancementMutex;

EnhancementSpell::EnhancementSpell()
    : Spell(0, 0) {}

SpellResult EnhancementSpell::use(Game& game, int targetX, int targetY) {
    Enhancement newEnhancement(1, 5, 1, 1);
    addPendingEnhancement(newEnhancement);
    
    Enhancement pending = getPendingEnhancement();

    std::string message =
        std::string("Enhancement spell cast! Next spell will be enhanced. ") +
        std::string("Pending bonuses: +") + std::to_string(pending.radiusBonus) + " radius, +" +
        std::to_string(pending.damageBonus) + " damage, +" +
        std::to_string(pending.areaBonus) + " area, +" +
        std::to_string(pending.summonBonus) + " summons.";

    return SpellResult(true, true, message);
}

std::string EnhancementSpell::getName() const {
    return "Enhancement";
}

std::string EnhancementSpell::getDescription() const {
    Enhancement pending = getPendingEnhancement();
    return std::string("Enhances the next spell used. Current pending: +") + 
           std::to_string(pending.radiusBonus) + " radius, +" + 
           std::to_string(pending.damageBonus) + " damage, +" + 
           std::to_string(pending.areaBonus) + " area, +" + 
           std::to_string(pending.summonBonus) + " summons.";
}

std::unique_ptr<Spell> EnhancementSpell::clone() const {
    return std::make_unique<EnhancementSpell>(*this);
}

Enhancement EnhancementSpell::getPendingEnhancement() {
    std::lock_guard<std::mutex> lock(enhancementMutex);
    return pendingEnhancement;
}

void EnhancementSpell::clearPendingEnhancement() {
    std::lock_guard<std::mutex> lock(enhancementMutex);
    pendingEnhancement = Enhancement();
}

void EnhancementSpell::addPendingEnhancement(const Enhancement& enh) {
    std::lock_guard<std::mutex> lock(enhancementMutex);
    pendingEnhancement += enh;
}
