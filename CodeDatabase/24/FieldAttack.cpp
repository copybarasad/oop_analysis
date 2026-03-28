#include "Field.hpp"
#include "Enemy.hpp"
#include "EnemyManager.hpp"
#include "Pos.hpp"

// Атака игрока в указанном направлении
bool Field::playerAttack(const Pos& direction) {
    int range = player.getAttackRange();
    bool hit = false;

    // Вычисляем целевую позицию для атаки
    Pos targetPos = playerPos + (direction * range);

    // Проверяем, что позиция в пределах поля
    if (!isValidPosition(targetPos)) {
        std::cout << "Attack missed: target position out of bounds." << std::endl;
        return false;
    }

    // Проверяем, что на пути нет препятствий
    if (!isCellPassable(targetPos)) {
        std::cout << "Attack blocked by obstacle." << std::endl;
        return false;
    }

    // Если на целевой позиции есть враг - атакуем его
    if (Enemy* enemy = enemyManager->getEnemyAt(targetPos)) {
        player.attack(enemy);
        hit = true;
    } else {
        std::cout << "Attack missed: no targets at range. " << range << std::endl;
    }

    // Удаляем мертвых врагов после атаки
    if (hit) {
        enemyManager->removeDeadEnemies();
    }

    return hit;
}
