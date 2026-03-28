#include "TargetFinder.h"
#include "Player.h"
#include "GameField.h"
#include "GameObject.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include "SpellPositioningSystem.h"
#include "SpellAreaSystem.h"
#include <algorithm>
#include <limits>

std::vector<GameObject*> TargetFinder::findTargetsInRange(const Player& caster, const GameField& field, int range) {
    std::vector<GameObject*> targets;

    for (const auto& enemy : caster.getEnemies()) {
        if (enemy.isAlive() && SpellPositioningSystem::calculateDistance(caster, enemy.getPositionX(), enemy.getPositionY()) <= range) {
            targets.push_back(const_cast<GameObject*>(static_cast<const GameObject*>(&enemy)));
        }
    }

    for (const auto& tower : caster.getTowers()) {
        if (tower.isAlive() && SpellPositioningSystem::calculateDistance(caster, tower.getPositionX(), tower.getPositionY()) <= range) {
            targets.push_back(const_cast<GameObject*>(static_cast<const GameObject*>(&tower)));
        }
    }

    return targets;
}

GameObject* TargetFinder::findClosestTarget(const Player& caster, const GameField& field, int range) {
    auto targets = findTargetsInRange(caster, field, range);
    if (targets.empty()) return nullptr;

    GameObject* closestTarget = targets[0];
    int minDistance = std::numeric_limits<int>::max();

    for (auto* target : targets) {
        int distance = SpellPositioningSystem::calculateDistance(caster, target->getPositionX(), target->getPositionY());
        if (distance < minDistance) {
            minDistance = distance;
            closestTarget = target;
        }
    }

    return closestTarget;
}

std::pair<int, int> TargetFinder::findBestAreaForDamage(const Player& caster, const GameField& field, int range) {
    return SpellAreaSystem::findOptimalArea(caster, field, range, 2);
}

std::vector<std::pair<int, int>> TargetFinder::findAvailableTrapPositions(const Player& caster, const GameField& field, int range) {
    auto positions = SpellPositioningSystem::getValidSpellPositions(caster, field, range, true);

    // Фильтруем позиции, где уже есть ловушки
    std::vector<std::pair<int, int>> availablePositions;
    for (const auto& pos : positions) {
        if (!field.hasTrap(pos.first, pos.second)) {
            availablePositions.push_back(pos);
        }
    }

    return availablePositions;
}