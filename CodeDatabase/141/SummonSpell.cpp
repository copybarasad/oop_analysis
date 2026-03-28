#include "SummonSpell.h"
#include "ISpellContext.h"
#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "Ally.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include "Trap.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <utility>
#include <ostream>

SummonSpell::SummonSpell(int count)
    : summonCount(count),
      name("Summon Ally"),
      description("Summons " + std::to_string(count) + (count == 1 ? " ally" : " allies")) {
    if (count <= 0) {
        throw std::invalid_argument("Count must be positive");
    }
}

bool SummonSpell::cast(ISpellContext &context) {
    Field &field = context.getField();
    Player &player = context.getPlayer();
    auto &enemies = context.getEnemies();
    auto &allies = context.getAllies();
    auto &buildings = context.getBuildings();
    const auto &towers = context.getTowers();
    auto &traps = context.getTraps();

    int count = summonCount;
    int alliesSummoned = 0;

    for (int i = 0; i < count; ++i) {
        int playerX, playerY;
        player.getPosition(playerX, playerY);

        std::vector<std::pair<int, int> > directions = {
            {0, 1}, {1, 0}, {0, -1}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
        };

        bool spawned = false;
        for (const auto &dir: directions) {
            int spawnX = playerX + dir.first;
            int spawnY = playerY + dir.second;

            if (field.isValidPosition(spawnX, spawnY) &&
                field.isPassable(spawnX, spawnY)) {
                bool occupied = false;
                if (spawnX == playerX && spawnY == playerY) occupied = true;

                for (const auto &enemy: enemies) {
                    int enemyX, enemyY;
                    enemy.getPosition(enemyX, enemyY);
                    if (spawnX == enemyX && spawnY == enemyY && enemy.isAlive()) {
                        occupied = true;
                        break;
                    }
                }

                for (const auto &ally: allies) {
                    int allyX, allyY;
                    ally.getPosition(allyX, allyY);
                    if (spawnX == allyX && spawnY == allyY && ally.isAlive()) {
                        occupied = true;
                        break;
                    }
                }

                if (!occupied) {
                    for (const auto &building: buildings) {
                        int bx, by;
                        building.getPosition(bx, by);
                        if (spawnX == bx && spawnY == by) {
                            occupied = true;
                            break;
                        }
                    }
                }

                if (!occupied) {
                    for (const auto &tower: towers) {
                        int tx, ty;
                        tower.getPosition(tx, ty);
                        if (spawnX == tx && spawnY == ty) {
                            occupied = true;
                            break;
                        }
                    }
                }

                if (!occupied) {
                    for (const auto &trap: traps) {
                        if (trap.getX() == spawnX && trap.getY() == spawnY && trap.isActive()) {
                            occupied = true;
                            break;
                        }
                    }
                }

                if (!occupied) {
                    allies.emplace_back(spawnX, spawnY,
                                        GameConstants::ALLY_BASE_HEALTH,
                                        GameConstants::ALLY_BASE_DAMAGE,
                                        field.getWidth(), field.getHeight());
                    alliesSummoned++;
                    context.addMessage(
                        "Ally summoned at (" + std::to_string(spawnX) + ", " + std::to_string(spawnY) + ")");
                    spawned = true;
                    break;
                }
            }
        }

        if (!spawned) {
            context.addMessage("No space to summon ally!");
        }
    }

    return alliesSummoned > 0;
}

bool SummonSpell::castAt(ISpellContext &context, int, int) {
    return cast(context);
}

std::string SummonSpell::getName() const {
    return name;
}

std::string SummonSpell::getDescription() const {
    return description;
}

SpellType SummonSpell::getType() const {
    return SpellType::SUMMON;
}

int SummonSpell::getSummonCount() const {
    return summonCount;
}

Spell *SummonSpell::clone() const {
    return new SummonSpell(*this);
}

Spell *SummonSpell::upgradedCopy() const {
    return new SummonSpell(summonCount + 1);
}

bool SummonSpell::requiresTarget() const {
    return false;
}

bool SummonSpell::enhance(const Enhancement &enhancement) {
    if (enhancement.type == EnhancementType::INCREASE_SUMMON_COUNT) {
        summonCount += enhancement.value;
        description = "Summons " + std::to_string(summonCount) + " allies";
        return true;
    }
    return false;
}

void SummonSpell::serialize(std::ostream &out) const {
    out << summonCount;
}
