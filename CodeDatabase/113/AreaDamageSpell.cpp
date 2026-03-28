#include "AreaDamageSpell.h"
#include "TargetFinder.h"
#include "SpellEffectSystem.h"
#include "SpellAreaSystem.h"
#include "SpellLoggingSystem.h"
#include "Player.h"
#include "GameField.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include <iostream>

AreaDamageSpell::AreaDamageSpell(const std::string& name, int manaCost, int range, int damage)
    : Spell(name, manaCost, range), m_damage(damage) {}

bool AreaDamageSpell::cast(Player& caster, GameField& field) {
    SpellLoggingSystem::logSpellSearch(m_name, "best 2x2 area");

    auto bestArea = TargetFinder::findBestAreaForDamage(caster, field, m_range);

    // Проверяем, что область содержит цели
    int targetCount = SpellAreaSystem::countTargetsInArea(caster, bestArea.first, bestArea.second, 2);
    if (targetCount == 0) {
        std::cout << "*** No targets found in area for " << m_name << std::endl;
        SpellLoggingSystem::logNoTargets(m_name);
        return false;
    }

    std::cout << "*** Casting " << m_name << " on area ("
        << bestArea.first << "," << bestArea.second << ") with "
        << targetCount << " targets" << std::endl;

    int hits = applyDamageInArea(caster, bestArea.first, bestArea.second);

    SpellLoggingSystem::logAreaSelection(m_name, bestArea.first, bestArea.second, hits);
    return true;
}

int AreaDamageSpell::applyDamageInArea(Player& caster, int areaX, int areaY) {
    int hits = 0;

    for (size_t i = 0; i < caster.getEnemies().size(); ++i) {
        auto& enemy = caster.getEnemies()[i];
        if (enemy.isAlive() && SpellAreaSystem::isPositionInArea(enemy.getPositionX(), enemy.getPositionY(), areaX, areaY, 2)) {
            std::cout << "*** Area spell hitting enemy at ("
                << enemy.getPositionX() << "," << enemy.getPositionY() << ")" << std::endl;
            SpellEffectSystem::applyDamageWithReward(caster, enemy, m_damage, m_name, "enemy");
            hits++;
        }
    }

    for (size_t i = 0; i < caster.getTowers().size(); ++i) {
        auto& tower = caster.getTowers()[i];
        if (tower.isAlive() && SpellAreaSystem::isPositionInArea(tower.getPositionX(), tower.getPositionY(), areaX, areaY, 2)) {
            std::cout << "*** Area spell hitting tower at ("
                << tower.getPositionX() << "," << tower.getPositionY() << ")" << std::endl;
            SpellEffectSystem::applyDamageToTarget(tower, m_damage, m_name, "tower");
            hits++;
        }
    }

    return hits;
}

std::string AreaDamageSpell::getDescription() const {
    return "Deals " + std::to_string(m_damage) + " damage to all targets in 2x2 area within " +
        std::to_string(m_range) + " tile radius (auto-targets area with most enemies)";
}