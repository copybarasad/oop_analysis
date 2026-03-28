#include "damage_spell.h"
#include "game_controller.h"
#include "enemy.h"

#include <cmath>
#include <fstream>
#include <string>
#include <cstdint>


DamageSpell::DamageSpell(const std::string& name,
    const std::string& description,
    int damage,
    int range,
    GameController* gameController)
    : spellName(name),
    spellDescription(description),
    spellDamage(damage),
    spellRange(range),
    gameController(gameController) {
}

bool DamageSpell::cast() {
    if (!canCast() || used) {
        return false;
    }

    bool result = applyEffect();
    if (result) {
        used = true;
    }
    return result;
}

bool DamageSpell::canCast() const {
    return !used && !getValidTargets().empty();
}

std::string DamageSpell::getName() const {
    return spellName;
}

std::string DamageSpell::getDescription() const {
    return spellDescription;
}

void DamageSpell::setTarget(const Position& target) {
    targetPosition = target;
}

bool DamageSpell::isValidTarget(const Position& target) const {
    const auto& enemies = gameController->getEnemies();

    for (const auto& enemy : enemies) {
        if (enemy.getPosition() == target && enemy.isAlive()) {
            Position playerPos = gameController->getPlayer().getPosition();
            int dx = std::abs(target.getX() - playerPos.getX());
            int dy = std::abs(target.getY() - playerPos.getY());
            int distance = std::max(dx, dy);

            return distance <= spellRange;
        }
    }

    return false;
}

std::vector<Position> DamageSpell::getValidTargets() const {
    std::vector<Position> validTargets;
    const auto& enemies = gameController->getEnemies();
    Position playerPos = gameController->getPlayer().getPosition();

    for (const auto& enemy : enemies) {
        if (!enemy.isAlive()) continue;

        Position enemyPos = enemy.getPosition();
        int dx = std::abs(enemyPos.getX() - playerPos.getX());
        int dy = std::abs(enemyPos.getY() - playerPos.getY());
        int distance = std::max(dx, dy);

        if (distance <= spellRange) {
            validTargets.push_back(enemyPos);
        }
    }

    return validTargets;
}

int DamageSpell::getRange() const {
    return spellRange;
}

int DamageSpell::getDamage() const {
    return spellDamage;
}

void DamageSpell::save(std::ostream& out) const {
    // Сохраняем строки: длина + данные
    uint16_t len = static_cast<uint16_t>(spellName.size());
    out.write(reinterpret_cast<const char*>(&len), sizeof(len));
    out.write(spellName.c_str(), len);

    len = static_cast<uint16_t>(spellDescription.size());
    out.write(reinterpret_cast<const char*>(&len), sizeof(len));
    out.write(spellDescription.c_str(), len);

    // Сохраняем числовые поля
    out.write(reinterpret_cast<const char*>(&spellDamage), sizeof(spellDamage));
    out.write(reinterpret_cast<const char*>(&spellRange), sizeof(spellRange));
    out.write(reinterpret_cast<const char*>(&used), sizeof(used));

    // Сохраняем позицию цели
    targetPosition.save(out);
}

void DamageSpell::load(std::istream& in) {
    uint16_t len;

    // Загружаем имя
    in.read(reinterpret_cast<char*>(&len), sizeof(len));
    spellName.resize(len);
    in.read(&spellName[0], len);

    // Загружаем описание
    in.read(reinterpret_cast<char*>(&len), sizeof(len));
    spellDescription.resize(len);
    in.read(&spellDescription[0], len);

    // Загружаем числовые поля
    in.read(reinterpret_cast<char*>(&spellDamage), sizeof(spellDamage));
    in.read(reinterpret_cast<char*>(&spellRange), sizeof(spellRange));
    in.read(reinterpret_cast<char*>(&used), sizeof(used));

    // Загружаем позицию цели
    targetPosition.load(in);
}