#include "PhysicalWorldSystem.h"
#include "CellContent.h"
bool PhysicalWorldSystem::processPlayerMove(GameState& state, int newX, int newY) {
    auto& field = state.getField();
    auto& player = state.getPlayer();

    if (!field.isValidPosition(newX, newY)) {
        std::cout << "Cannot move there - out of bounds!" << std::endl;
        return false;
    }

    if (attackEnemyAt(state, newX, newY)) {
        return true;
    }

    if (movePlayerTo(state, newX, newY)) {
        player.addScore(GameConstants::SCORE_FOR_MOVE);
        player.addMana(GameConstants::MANA_FOR_MOVE);
        return true;
    }

    return false;
}

bool PhysicalWorldSystem::attackEnemyAt(GameState& state, int x, int y) {
    auto& enemies = state.getEnemies();
    auto& player = state.getPlayer();

    for (auto& enemy : enemies) {
        if (enemy.isAlive() && enemy.getX() == x && enemy.getY() == y) {
            enemy.takeDamage(player.getDamage());
            std::cout << "You attacked enemy! Enemy health: "
                << enemy.getHealth() << std::endl;

            if (!enemy.isAlive()) {
                player.addScore(GameConstants::SCORE_FOR_KILL);
                std::cout << "Enemy defeated! +10 points" << std::endl;
            }
            return true;
        }
    }
    return false;
}

bool PhysicalWorldSystem::movePlayerTo(GameState& state, int newX, int newY) {
    return state.getField().movePlayer(newX, newY);
}

void PhysicalWorldSystem::processEnemyMoves(GameState& state) {
    auto& enemies = state.getEnemies();

    for (auto& enemy : enemies) {
        if (!enemy.isAlive()) continue;

        // Даже если враг рядом с игроком, он может попытаться переместиться
        // Например, с 50% вероятностью атаковать, с 50% попытаться двигаться
        if (enemyCanAttackPlayer(state, enemy)) {
            // Случайно решаем: атаковать или двигаться
            if (std::rand() % 2 == 0) {
                // Атаковать
                auto& player = state.getPlayer();
                player.takeDamage(enemy.getDamage());
                std::cout << "Enemy attacked you! Lost "
                    << enemy.getDamage() << " health!" << std::endl;
            }
            else {
                // Попытаться переместиться (может попасть на игрока и нанести урон)
                moveEnemyRandomly(state, enemy);
            }
        }
        else {
            // Враг далеко - просто двигаемся
            moveEnemyRandomly(state, enemy);
        }
    }
}

void PhysicalWorldSystem::moveEnemyRandomly(GameState& state, Enemy& enemy) {
    int randomNumber = std::rand() % 4;
    int oldX = enemy.getX();
    int oldY = enemy.getY();
    int newX = oldX;
    int newY = oldY;

    switch (randomNumber) {
    case 0: newX++; break;  // Вправо
    case 1: newX--; break;  // Влево
    case 2: newY++; break;  // Вниз
    case 3: newY--; break;  // Вверх
    }

    auto& field = state.getField();

    if (!field.isValidPosition(newX, newY)) {
        return;
    }

    char cellContent = field.getCellContent(newX, newY);

    // Если на клетке игрок
    if (cellContent == CellContent::PLAYER) {
        auto& player = state.getPlayer();
        player.takeDamage(enemy.getDamage());
        std::cout << "Enemy tried to move onto you and attacked! Lost "
            << enemy.getDamage() << " health!" << std::endl;
        return;  // Перемещение не происходит
    }

    // Если клетка пустая
    if (cellContent == CellContent::EMPTY) {
        enemy.move(newX, newY);
    }

    // Если клетка занята врагом (CellContent::ENEMY) - не двигаемся
}

bool PhysicalWorldSystem::enemyCanAttackPlayer(const GameState& state, const Enemy& enemy) {
    auto playerPos = state.getField().getPlayerPosition();
    int dx = std::abs(enemy.getX() - playerPos.first);
    int dy = std::abs(enemy.getY() - playerPos.second);
    return dx <= 1 && dy <= 1;
}
