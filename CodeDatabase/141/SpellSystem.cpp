#include "SpellSystem.h"

SpellSystem::SpellSystem() = default;

Spell *SpellSystem::createRandomSpell() {
    return spellFactory.createRandomSpell();
}

bool SpellSystem::castSpell(Spell *spell, ISpellContext &context) {
    if (!spell) return false;

    if (spell->getType() != SpellType::ENHANCE) {
        applyEnhancements(*spell, [&context](const std::string &msg) { context.addMessage(msg); });
    }

    return spell->cast(context);
}

bool SpellSystem::castSpellAt(Spell *spell, ISpellContext &context, int targetX, int targetY) {
    if (!spell) return false;

    if (spell->getType() != SpellType::ENHANCE) {
        applyEnhancements(*spell, [&context](const std::string &msg) { context.addMessage(msg); });
    }

    return spell->castAt(context, targetX, targetY);
}

bool SpellSystem::castTowerSpell(int sourceX, int sourceY, int targetX, int targetY, int range, int damage,
                                 Field &field, Player &player, std::vector<Enemy> &enemies,
                                 std::vector<EnemyBuilding> &buildings,
                                 std::function<void(const std::string &)> addMessage) {
    (void) buildings;
    int dx = 0, dy = 0;
    if (targetX != sourceX) dx = (targetX > sourceX) ? 1 : -1;
    if (targetY != sourceY) dy = (targetY > sourceY) ? 1 : -1;

    bool hitSomething = false;

    for (int step = 1; step <= range; ++step) {
        int checkX = sourceX + dx * step;
        int checkY = sourceY + dy * step;

        if (!field.isValidPosition(checkX, checkY)) break;

        if (!field.isPassable(checkX, checkY)) {
            addMessage("Башня попала заклинанием в препятствие");
            hitSomething = true;
            break;
        }

        int playerX, playerY;
        player.getPosition(playerX, playerY);
        if (checkX == playerX && checkY == playerY) {
            player.takeDamage(damage);
            addMessage("Вражеская башня попала по вам на " + std::to_string(damage) + " урона");
            hitSomething = true;
            break;
        }

        for (auto &enemy: enemies) {
            int enemyX, enemyY;
            enemy.getPosition(enemyX, enemyY);
            if (enemyX == checkX && enemyY == checkY && enemy.isAlive()) {
                enemy.takeDamage(damage);
                addMessage("Башня случайно задела врага");
                hitSomething = true;
                break;
            }
        }

        if (hitSomething) break;
    }

    return hitSomething;
}

void SpellSystem::addEnhancement(const Enhancement &enhancement) {
    pendingEnhancements.push_back(enhancement);
}

void SpellSystem::applyEnhancements(Spell &spell, std::function<void(const std::string &)> addMessage) {
    if (pendingEnhancements.empty()) return;

    for (const auto &enhancement: pendingEnhancements) {
        if (spell.enhance(enhancement)) {
            std::string typeStr;
            switch (enhancement.type) {
                case EnhancementType::INCREASE_RANGE: typeStr = "радиус";
                    break;
                case EnhancementType::INCREASE_AREA: typeStr = "площадь";
                    break;
                case EnhancementType::INCREASE_DAMAGE: typeStr = "урон";
                    break;
                case EnhancementType::INCREASE_SUMMON_COUNT: typeStr = "призыв";
                    break;
            }
            addMessage("Усилено: +" + std::to_string(enhancement.value) + " " + typeStr);
        }
    }


    pendingEnhancements.clear();
}

void SpellSystem::clearEnhancements() {
    pendingEnhancements.clear();
}

std::vector<Enhancement> SpellSystem::getPendingEnhancements() const {
    return pendingEnhancements;
}
