#include "DirectDamageSpell.h"
#include "ISpellContext.h"
#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "GameConstants.h"
#include <string>
#include <ostream>

DirectDamageSpell::DirectDamageSpell(int damage, int range)
    : damage(damage), range(range),
      name("Fire Bolt"),
      description("Deals " + std::to_string(damage) + " damage to a single target") {
    if (damage <= 0 || range <= 0) {
        throw std::invalid_argument("Damage and range must be positive");
    }
}

bool DirectDamageSpell::cast(ISpellContext &) {
    return false;
}

bool DirectDamageSpell::castAt(ISpellContext &context, int targetX, int targetY) {
    Field &field = context.getField();
    Player &player = context.getPlayer();
    auto &enemies = context.getEnemies();
    auto &buildings = context.getBuildings();

    int playerX, playerY;
    player.getPosition(playerX, playerY);

    int dx = 0, dy = 0;
    if (targetX != playerX) dx = (targetX > playerX) ? 1 : -1;
    if (targetY != playerY) dy = (targetY > playerY) ? 1 : -1;

    bool hitSomething = false;
    int currentDamage = damage;

    for (int step = 1; step <= range; ++step) {
        int checkX = playerX + dx * step;
        int checkY = playerY + dy * step;

        if (!field.isValidPosition(checkX, checkY)) {
            context.addMessage("Огненный заряд достиг края карты");
            break;
        }

        if (!field.isPassable(checkX, checkY)) {
            context.addMessage("Огненный заряд встретил препятствие в точке (" +
                               std::to_string(checkX) + ", " + std::to_string(checkY) + ")");
            hitSomething = true;
            break;
        }

        Enemy *enemy = nullptr;
        for (auto &e: enemies) {
            int enemyX, enemyY;
            e.getPosition(enemyX, enemyY);
            if (enemyX == checkX && enemyY == checkY && e.isAlive()) {
                enemy = &e;
                break;
            }
        }

        if (enemy && enemy->isAlive()) {
            enemy->takeDamage(currentDamage);
            context.addMessage("Огненный заряд попал по врагу на дистанции " + std::to_string(step) +
                               " на " + std::to_string(currentDamage) + " урона");
            hitSomething = true;

            if (!enemy->isAlive()) {
                context.addScore();
                context.incrementKills(1);
                context.addMessage("Враг побеждён! +" + std::to_string(GameConstants::SCORE_PER_KILL) + " очков");
            }
            break;
        }

        EnemyBuilding *building = nullptr;
        for (auto &b: buildings) {
            int buildingX, buildingY;
            b.getPosition(buildingX, buildingY);
            if (buildingX == checkX && buildingY == checkY) {
                building = &b;
                break;
            }
        }

        if (building) {
            context.addMessage("Огненный заряд попал по зданию (" +
                               std::to_string(checkX) + ", " + std::to_string(checkY) + ")");
            hitSomething = true;
            break;
        }

        if (step == range && !hitSomething) {
            context.addMessage("Огненный заряд никого не задел");
            hitSomething = true;
        }
    }

    return hitSomething;
}

std::string DirectDamageSpell::getName() const {
    return name;
}

std::string DirectDamageSpell::getDescription() const {
    return description;
}

SpellType DirectDamageSpell::getType() const {
    return SpellType::DIRECT_DAMAGE;
}

int DirectDamageSpell::getRange() const {
    return range;
}

int DirectDamageSpell::getDamage() const {
    return damage;
}

Spell *DirectDamageSpell::clone() const {
    return new DirectDamageSpell(*this);
}

Spell *DirectDamageSpell::upgradedCopy() const {
    return new DirectDamageSpell(damage + 5, range + 1);
}

bool DirectDamageSpell::requiresTarget() const {
    return true;
}

bool DirectDamageSpell::enhance(const Enhancement &enhancement) {
    if (enhancement.type == EnhancementType::INCREASE_RANGE) {
        range += enhancement.value;
        return true;
    } else if (enhancement.type == EnhancementType::INCREASE_DAMAGE) {
        damage += enhancement.value;
        description = "Deals " + std::to_string(damage) + " damage to a single target";
        return true;
    }
    return false;
}

void DirectDamageSpell::serialize(std::ostream &out) const {
    out << damage << " " << range;
}
