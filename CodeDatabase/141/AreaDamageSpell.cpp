#include "AreaDamageSpell.h"
#include "ISpellContext.h"
#include "Field.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "GameConstants.h"
#include <string>
#include <ostream>

AreaDamageSpell::AreaDamageSpell(int damage, int range)
    : damage(damage), range(range), areaSize(2),
      name("Fireball"),
      description(
          "Deals " + std::to_string(damage) + " damage in " + std::to_string(areaSize) + "x" + std::to_string(areaSize)
          + " area") {
    if (damage <= 0 || range <= 0 || areaSize <= 0) {
        throw std::invalid_argument("Damage, range and area size must be positive");
    }
}

bool AreaDamageSpell::cast(ISpellContext &) {
    return false;
}

bool AreaDamageSpell::castAt(ISpellContext &context, int targetX, int targetY) {
    Field &field = context.getField();
    auto &enemies = context.getEnemies();
    auto &buildings = context.getBuildings();

    bool hitSomething = false;
    int currentDamage = damage;
    int currentAreaSize = areaSize;

    for (int dx = 0; dx < currentAreaSize; ++dx) {
        for (int dy = 0; dy < currentAreaSize; ++dy) {
            int checkX = targetX + dx;
            int checkY = targetY + dy;

            if (!field.isValidPosition(checkX, checkY)) continue;

            for (auto &enemy: enemies) {
                int enemyX, enemyY;
                enemy.getPosition(enemyX, enemyY);
                if (enemyX == checkX && enemyY == checkY && enemy.isAlive()) {
                    enemy.takeDamage(currentDamage);
                    hitSomething = true;
                    context.addMessage("Огненный шар задел врага (" + std::to_string(checkX) + "," +
                                       std::to_string(checkY) + ") на " + std::to_string(currentDamage) + " урона");

                    if (!enemy.isAlive()) {
                        context.addScore();
                        context.incrementKills(1);
                        context.addMessage(
                            "Враг побеждён! +" + std::to_string(GameConstants::SCORE_PER_KILL) + " очков");
                    }
                }
            }

            for (auto &building: buildings) {
                int buildingX, buildingY;
                building.getPosition(buildingX, buildingY);
                if (buildingX == checkX && buildingY == checkY) {
                    context.addMessage("Огненный шар задел вражеское здание (" +
                                       std::to_string(checkX) + "," + std::to_string(checkY) + ")");
                    hitSomething = true;
                }
            }
        }
    }

    if (!hitSomething) {
        context.addMessage("Огненный шар взорвался в (" + std::to_string(targetX) + ", " +
                           std::to_string(targetY) + "), но никого не задел");
    } else {
        context.addMessage("Взрыв огненного шара завершён");
    }
    return true;
}

std::string AreaDamageSpell::getName() const {
    return name;
}

std::string AreaDamageSpell::getDescription() const {
    return description;
}

SpellType AreaDamageSpell::getType() const {
    return SpellType::AREA_DAMAGE;
}

int AreaDamageSpell::getRange() const {
    return range;
}

int AreaDamageSpell::getDamage() const {
    return damage;
}

int AreaDamageSpell::getAreaSize() const {
    return areaSize;
}

Spell *AreaDamageSpell::clone() const {
    return new AreaDamageSpell(*this);
}

Spell *AreaDamageSpell::upgradedCopy() const {
    return new AreaDamageSpell(damage + 5, range + 1);
}

bool AreaDamageSpell::requiresTarget() const {
    return true;
}

bool AreaDamageSpell::enhance(const Enhancement &enhancement) {
    if (enhancement.type == EnhancementType::INCREASE_RANGE) {
        range += enhancement.value;
        return true;
    } else if (enhancement.type == EnhancementType::INCREASE_DAMAGE) {
        damage += enhancement.value;
        description = "Deals " + std::to_string(damage) + " damage in " + std::to_string(areaSize) + "x" +
                      std::to_string(areaSize) + " area";
        return true;
    } else if (enhancement.type == EnhancementType::INCREASE_AREA) {
        areaSize += enhancement.value;
        description = "Deals " + std::to_string(damage) + " damage in " + std::to_string(areaSize) + "x" +
                      std::to_string(areaSize) + " area";
        return true;
    }
    return false;
}

void AreaDamageSpell::serialize(std::ostream &out) const {
    out << damage << " " << range << " " << areaSize;
}
