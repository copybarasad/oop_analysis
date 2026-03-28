#include "TrapSpell.h"
#include "ISpellContext.h"
#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "Trap.h"
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ostream>

TrapSpell::TrapSpell(int trapDamage, int trapRange)
    : damage(trapDamage), range(trapRange),
      name("Trap"),
      description("Ставит ловушку, наносящую " + std::to_string(damage) + " урона при срабатывании") {
    if (trapDamage <= 0 || trapRange <= 0) {
        throw std::invalid_argument("Damage and range must be positive");
    }
}

bool TrapSpell::cast(ISpellContext &) {
    return false;
}

bool TrapSpell::castAt(ISpellContext &context, int targetX, int targetY) {
    Field &field = context.getField();
    Player &player = context.getPlayer();
    auto &enemies = context.getEnemies();
    auto &traps = context.getTraps();

    int playerX, playerY;
    player.getPosition(playerX, playerY);
    int distanceX = std::abs(targetX - playerX);
    int distanceY = std::abs(targetY - playerY);
    if (std::max(distanceX, distanceY) > range) {
        context.addMessage("Цель для ловушки вне диапазона");
        return false;
    }

    if (field.isValidPosition(targetX, targetY) && field.isPassable(targetX, targetY)) {
        bool positionOccupied = false;

        if (targetX == playerX && targetY == playerY) positionOccupied = true;

        for (const auto &enemy: enemies) {
            int enemyX, enemyY;
            enemy.getPosition(enemyX, enemyY);
            if (targetX == enemyX && targetY == enemyY && enemy.isAlive()) {
                positionOccupied = true;
                break;
            }
        }

        for (const auto &trap: traps) {
            if (trap.getX() == targetX && trap.getY() == targetY && trap.isActive()) {
                positionOccupied = true;
                break;
            }
        }

        if (!positionOccupied) {
            int currentDamage = damage;
            traps.emplace_back(targetX, targetY, currentDamage);
            context.addMessage(
                "Ловушка поставлена в (" + std::to_string(targetX) + ", " + std::to_string(targetY) + ")");
            return true;
        }
    }
    context.addMessage("Нельзя поставить ловушку в эту клетку");
    return false;
}

std::string TrapSpell::getName() const {
    return name;
}

std::string TrapSpell::getDescription() const {
    return description;
}

SpellType TrapSpell::getType() const {
    return SpellType::TRAP;
}

int TrapSpell::getRange() const {
    return range;
}

int TrapSpell::getDamage() const {
    return damage;
}

Spell *TrapSpell::clone() const {
    return new TrapSpell(*this);
}

Spell *TrapSpell::upgradedCopy() const {
    return new TrapSpell(damage + 5, range + 1);
}

bool TrapSpell::requiresTarget() const {
    return true;
}

bool TrapSpell::enhance(const Enhancement &enhancement) {
    if (enhancement.type == EnhancementType::INCREASE_DAMAGE) {
        damage += enhancement.value;
        description = "Ставит ловушку, наносящую " + std::to_string(damage) + " урона при срабатывании";
        return true;
    }
    return false;
}

void TrapSpell::serialize(std::ostream &out) const {
    out << damage << " " << range;
}
