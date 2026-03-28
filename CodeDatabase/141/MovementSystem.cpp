#include "MovementSystem.h"

const std::vector<std::pair<int, int> > MovementSystem::DIRECTIONS = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

MovementSystem::MovementSystem() = default;

bool MovementSystem::movePlayer(Player &player, int dx, int dy, const Field &field,
                                const std::vector<Enemy> &enemies,
                                const std::vector<EnemyBuilding> &buildings,
                                const std::vector<EnemyTower> &towers,
                                const std::vector<Ally> &allies,
                                std::function<void(const std::string &)> addMessage) {
    int currentX, currentY;
    player.getPosition(currentX, currentY);

    int newX = currentX + dx;
    int newY = currentY + dy;

    if (!field.isValidPosition(newX, newY)) {
        addMessage("Cannot move outside the map!");
        return false;
    }

    if (field.isPassable(newX, newY) &&
        isPositionFree(newX, newY, field, player, enemies, buildings, towers, allies)) {
        player.setPosition(newX, newY);

        if (field.getCell(newX, newY).isSlow()) {
            player.setSkipNextTurn(true);
            addMessage("Stepped on slowing terrain! Will skip next turn.");
        }

        return true;
    } else {
        addMessage("Cannot move there! Cell is blocked or occupied.");
        return false;
    }
}

void MovementSystem::moveEnemy(Enemy &enemy, const Field &field, Player &player,
                               const std::vector<Enemy> &otherEnemies,
                               const std::vector<EnemyBuilding> &buildings,
                               const std::vector<EnemyTower> &towers,
                               const std::vector<Ally> &allies,
                               std::function<void(const std::string &)> addMessage,
                               std::function<void(Enemy &, Player &)> attackCallback) {
    (void) addMessage;
    int enemyX, enemyY;
    enemy.getPosition(enemyX, enemyY);

    int directionIndex = std::rand() % DIRECTIONS.size();
    int dx = DIRECTIONS[directionIndex].first;
    int dy = DIRECTIONS[directionIndex].second;

    int newX = enemyX + dx;
    int newY = enemyY + dy;

    int playerX, playerY;
    player.getPosition(playerX, playerY);
    if (newX == playerX && newY == playerY) {
        attackCallback(enemy, player);
        return;
    }

    if (field.isValidPosition(newX, newY) &&
        field.isPassable(newX, newY) &&
        isPositionFree(newX, newY, field, player, otherEnemies, buildings, towers, allies)) {
        enemy.setPosition(newX, newY);
    }
}

void MovementSystem::moveAlly(Ally &ally, const Field &field,
                              const std::vector<Enemy> &enemies,
                              const std::vector<EnemyBuilding> &buildings,
                              const std::vector<EnemyTower> &towers,
                              const std::vector<Ally> &otherAllies,
                              const Player &player) {
    int allyX, allyY;
    ally.getPosition(allyX, allyY);

    int directionIndex = std::rand() % DIRECTIONS.size();
    int dx = DIRECTIONS[directionIndex].first;
    int dy = DIRECTIONS[directionIndex].second;

    int newX = allyX + dx;
    int newY = allyY + dy;

    if (field.isValidPosition(newX, newY) &&
        field.isPassable(newX, newY) &&
        isPositionFree(newX, newY, field, player, enemies, buildings, towers, otherAllies)) {
        ally.setPosition(newX, newY);
    }
}

bool MovementSystem::isPositionFree(int x, int y, const Field &field,
                                    const Player &player,
                                    const std::vector<Enemy> &enemies,
                                    const std::vector<EnemyBuilding> &buildings,
                                    const std::vector<EnemyTower> &towers,
                                    const std::vector<Ally> &allies) const {
    if (!field.isValidPosition(x, y)) return false;

    int playerX, playerY;
    player.getPosition(playerX, playerY);
    if (x == playerX && y == playerY) return false;

    for (const auto &enemy: enemies) {
        int enemyX, enemyY;
        enemy.getPosition(enemyX, enemyY);
        if (x == enemyX && y == enemyY && enemy.isAlive()) return false;
    }

    for (const auto &building: buildings) {
        int buildingX, buildingY;
        building.getPosition(buildingX, buildingY);
        if (x == buildingX && y == buildingY) return false;
    }

    for (const auto &tower: towers) {
        int towerX, towerY;
        tower.getPosition(towerX, towerY);
        if (x == towerX && y == towerY) return false;
    }

    for (const auto &ally: allies) {
        int allyX, allyY;
        ally.getPosition(allyX, allyY);
        if (x == allyX && y == allyY && ally.isAlive()) return false;
    }

    return true;
}
