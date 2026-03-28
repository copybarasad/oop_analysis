#include "TrapSpell.h"
#include "TargetFinder.h"
#include "RandomPositionSystem.h"
#include "SpellLoggingSystem.h"
#include "Player.h"
#include "GameField.h"
#include "Enemy.h"
#include <iostream>
#include <cmath>

TrapSpell::TrapSpell(const std::string& name, int manaCost, int range, int damage)
    : Spell(name, manaCost, range), m_damage(damage) {}

bool TrapSpell::cast(Player& caster, GameField& field) {
    auto availablePositions = TargetFinder::findAvailableTrapPositions(caster, field, getRange());

    if (availablePositions.empty()) {
        SpellLoggingSystem::logNoValidPositions(getName());
        return false;
    }

    auto selectedPos = RandomPositionSystem::selectRandomPosition(availablePositions);

    // ѕровер€ем возможность установки ловушки
    if (canPlaceTrap(caster, field, selectedPos.first, selectedPos.second)) {
        field.placeTrap(selectedPos.first, selectedPos.second, m_damage);
        SpellLoggingSystem::logTrapPlacement(getName(), selectedPos.first, selectedPos.second);
        return true;
    }

    return false;
}

std::string TrapSpell::getDescription() const {
    return "Places " + getTrapType() + " trap dealing " + std::to_string(m_damage) + " damage when triggered";
}

int TrapSpell::getTrapDamage() const {
    return m_damage;
}

bool TrapSpell::canPlaceTrap(const Player& caster, const GameField& field, int x, int y) const {
    // ѕровер€ем, что позици€ валидна и проходима
    if (!field.isValidPosition(x, y) || !field.isCellPassable(x, y)) {
        return false;
    }

    // ѕровер€ем, что на позиции нет другой ловушки
    if (field.hasTrap(x, y)) {
        return false;
    }

    // ѕровер€ем, что позици€ находитс€ в пределах дальности заклинани€
    int distance = std::abs(x - caster.getPositionX()) + std::abs(y - caster.getPositionY());
    if (distance > getRange()) {
        return false;
    }

    return true;
}

void TrapSpell::onTrapTriggered(int x, int y, GameObject& trigger) const {
    std::cout << getTrapType() << " trap at (" << x << "," << y << ") was triggered by ";

    if (dynamic_cast<Player*>(&trigger)) {
        std::cout << "player!" << std::endl;
    }
    else if (dynamic_cast<Enemy*>(&trigger)) {
        std::cout << "enemy!" << std::endl;
    }
    else {
        std::cout << "unknown creature!" << std::endl;
    }
}

std::string TrapSpell::getTrapType() const {
    return "Bear";
}