#include "DirectDamageSpell.h"
#include "TargetFinder.h"
#include "SpellEffectSystem.h"
#include "SpellExceptionHandler.h"
#include "SpellLoggingSystem.h"
#include "Player.h"
#include "GameField.h"
#include "GameObject.h"

DirectDamageSpell::DirectDamageSpell(const std::string& name, int manaCost, int range, int damage)
    : Spell(name, manaCost, range), m_damage(damage) {}

bool DirectDamageSpell::cast(Player& caster, GameField& field) {
    auto castOperation = [&]() -> bool {
        GameObject* target = TargetFinder::findClosestTarget(caster, field, m_range);
        if (!target) {
            SpellLoggingSystem::logNoTargets(m_name);
            return false;
        }

        std::cout << "*** Casting " << m_name << " on target at ("
            << target->getPositionX() << "," << target->getPositionY()
            << ") with " << target->getHealth() << " HP" << std::endl;

        SpellEffectSystem::applyDamageWithReward(caster, *target, m_damage, m_name);
        return true;
        };

    return SpellExceptionHandler::handleSpellCast(castOperation, m_name);
}

std::string DirectDamageSpell::getDescription() const {
    return "Deals " + std::to_string(m_damage) + " damage to closest target in range " + std::to_string(m_range);
}