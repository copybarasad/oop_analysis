#include "CombatSystem.h"
#include "GameConstants.h"

const std::vector<std::pair<int, int> > CombatSystem::DIRECTIONS = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

CombatSystem::CombatSystem() = default;

Enemy *findEnemyAtPosition(std::vector<Enemy> &enemies, int x, int y) {
    for (auto &enemy: enemies) {
        int enemyX, enemyY;
        enemy.getPosition(enemyX, enemyY);
        if (enemyX == x && enemyY == y && enemy.isAlive()) {
            return &enemy;
        }
    }
    return nullptr;
}

void CombatSystem::performMeleeAttack(Player &player, const Field &field,
                                      std::vector<Enemy> &enemies, int dx, int dy,
                                      std::function<void(const std::string &)> addMessage,
                                      std::function<void()> addScore) {
    int playerX, playerY;
    player.getPosition(playerX, playerY);

    int targetX = playerX + dx;
    int targetY = playerY + dy;

    if (!field.isValidPosition(targetX, targetY)) {
        addMessage("Attack direction is outside the map!");
        return;
    }

    Enemy *enemy = findEnemyAtPosition(enemies, targetX, targetY);
    if (enemy != nullptr) {
        int damage = player.getMeleeDamage();
        enemy->takeDamage(damage);
        addMessage("Удар ближним боем нанёс врагу " + std::to_string(damage) + " урона");

        if (!enemy->isAlive()) {
            addScore();
            addMessage("Враг побеждён! +" + std::to_string(GameConstants::SCORE_PER_KILL) + " очков");
        } else {
            addMessage("Здоровье врага: " + std::to_string(enemy->getHealth()));
        }
    } else {
        addMessage("Удар прошёл впустую");
    }
}

void CombatSystem::performRangedAttack(Player &player, const Field &field,
                                       std::vector<Enemy> &enemies, int dx, int dy,
                                       std::function<void(const std::string &)> addMessage,
                                       std::function<void()> addScore) {
    int playerX, playerY;
    player.getPosition(playerX, playerY);

    for (int distance = 1; distance <= GameConstants::RANGED_ATTACK_RANGE; ++distance) {
        int targetX = playerX + (dx * distance);
        int targetY = playerY + (dy * distance);

        if (!field.isValidPosition(targetX, targetY)) {
            addMessage("Дальний удар упёрся в край карты");
            return;
        }

        if (!field.isPassable(targetX, targetY)) {
            addMessage("Дальний удар встретил препятствие");
            return;
        }

        Enemy *enemy = findEnemyAtPosition(enemies, targetX, targetY);
        if (enemy != nullptr) {
            int damage = player.getRangedDamage();
            enemy->takeDamage(damage);
            addMessage("Дальний удар по врагу на дистанции " + std::to_string(distance) +
                       " на " + std::to_string(damage) + " урона");

            if (!enemy->isAlive()) {
                addScore();
                addMessage("Враг побеждён! +" + std::to_string(GameConstants::SCORE_PER_KILL) + " очков");
            } else {
                addMessage("Здоровье врага: " + std::to_string(enemy->getHealth()));
            }
            return;
        }
    }

    addMessage("Дальний удар прошёл впустую");
}

void CombatSystem::handleEnemyAttack(Enemy &enemy, Player &player,
                                     std::function<void(const std::string &)> addMessage) {
    int enemyDamage = enemy.getDamage();
    player.takeDamage(enemyDamage);
    addMessage("Враг атаковал на " + std::to_string(enemyDamage) +
               " урона. Текущее здоровье: " + std::to_string(player.getHealth()));
}
