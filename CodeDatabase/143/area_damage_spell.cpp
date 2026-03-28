#include "area_damage_spell.h"
#include "game_controller.h"
#include "enemy.h"
#include <algorithm>
#include <iostream>
#include <fstream>

AreaDamageSpell::AreaDamageSpell(const std::string& name,
    const std::string& description,
    int damage,
    int range,
    int areaSize,
    GameController* gameController)
    : DamageSpell(name, description, damage, range, gameController),
    areaSize(areaSize) {
}

bool AreaDamageSpell::applyEffect() {
    if (!isValidTarget(targetPosition)) {
        return false;
    }

    auto areaCells = getAreaCells(targetPosition);
    auto& enemies = const_cast<std::vector<Enemy>&>(gameController->getEnemies());
    bool hitAnyEnemy = false;

    for (auto& enemy : enemies) {
        if (!enemy.isAlive()) continue;

        Position enemyPos = enemy.getPosition();
        for (const auto& cell : areaCells) {
            if (enemyPos == cell) {
                enemy.takeDamage(spellDamage);
                hitAnyEnemy = true;
                std::cout << "Hit enemy at (" << enemyPos.getX() << ", " << enemyPos.getY() << ") for " << spellDamage << " damage!\n";
                break;
            }
        }
    }

    if (!hitAnyEnemy) {
        std::cout << "Area spell cast but no enemies in the target area.\n";
    }

    return true;
}

std::vector<Position> AreaDamageSpell::getAreaCells(const Position& center) const {
    std::vector<Position> areaCells;

    int startX = std::max(0, center.getX());
    int startY = std::max(0, center.getY());
    int endX = std::min(gameController->getGameField().getWidth(), startX + areaSize);
    int endY = std::min(gameController->getGameField().getHeight(), startY + areaSize);

    for (int y = startY; y < endY; ++y) {
        for (int x = startX; x < endX; ++x) {
            areaCells.push_back(Position(x, y));
        }
    }

    return areaCells;
}

uint8_t AreaDamageSpell::getType() const {
    return 2;  // 2 = урон по области
}

void AreaDamageSpell::save(std::ostream& out) const {
    DamageSpell::save(out);     // базовые поля
    out.write(reinterpret_cast<const char*>(&areaSize), sizeof(areaSize));  // доп. поле
}

void AreaDamageSpell::load(std::istream& in) {
    DamageSpell::load(in);
    in.read(reinterpret_cast<char*>(&areaSize), sizeof(areaSize));
}