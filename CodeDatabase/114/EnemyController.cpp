#include "EnemyController.h"
#include "../models/events/EventNotifier.h"
#include "../models/events/TrapTriggeredEvent.h"
#include "../models/events/EntityDeathEvent.h"
#include "../models/events/EnemySpawnedEvent.h"
#include "Cell.h"
#include <random>

void EnemyController::processEnemyTurn() {
    if (!field) return;
    auto& enemies = field->getEnemies();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-1, 1);
    
    // Используем обратную итерацию для безопасного удаления мертвых врагов
    for (auto it = enemies.rbegin(); it != enemies.rend(); ++it) {
        if (!it->isAlive()) {
            // Удаляем мертвого врага
            int x = it->getX();
            int y = it->getY();
            if (field->isValidPosition(x, y)) {
                field->getCells()[y][x].setHasEntity(false);
            }
            enemies.erase(std::next(it).base());
            continue;
        }
        
        int oldX = it->getX();
        int oldY = it->getY();
        int nx = oldX + dist(gen);
        int ny = oldY + dist(gen);
        if (field->isValidPosition(nx, ny) && field->isPositionEmpty(nx, ny)) {
            if (field->getCells()[ny][nx].getType() == CellType::TRAP) {
                int trapDamage = field->getCells()[ny][nx].getTrapDamage();
                // Уведомляем о срабатывании ловушки
                TrapTriggeredEvent trapEvent(it->getEntityName(), trapDamage, nx, ny);
                EventNotifier::getInstance().notify(trapEvent);
                
                it->takeDamage(trapDamage);
                field->getCells()[ny][nx].setType(CellType::EMPTY);
                field->getCells()[ny][nx].setTrapDamage(TRAP_DAMAGE);
                
                // Если враг умер от ловушки, удаляем его
                if (!it->isAlive()) {
                    // Уведомляем о смерти сущности
                    EntityDeathEvent deathEvent(it->getEntityName(), nx, ny);
                    EventNotifier::getInstance().notify(deathEvent);
                    
                    field->getCells()[oldY][oldX].setHasEntity(false);
                    enemies.erase(std::next(it).base());
                    continue;
                }
            }
            field->getCells()[oldY][oldX].setHasEntity(false);
            field->getCells()[ny][nx].setHasEntity(true);
            it->setPosition(nx, ny);
        }
    }
}

int EnemyController::calculateEnemyHealth(int level) const {
    // Базовое здоровье 50, увеличивается на 20 за каждый уровень
    return 50 + (level - 1) * 20;
}

int EnemyController::calculateEnemyDamage(int level) const {
    // Базовый урон 15, увеличивается на 5 за каждый уровень
    return 15 + (level - 1) * 5;
}

void EnemyController::generateRandomEnemies() {
    std::random_device rd;
    std::mt19937 gen(rd());
    int width = field->getWidth();
    int height = field->getHeight();

    std::uniform_int_distribution<> rndX(0, width - 1);
    std::uniform_int_distribution<> rndY(0, height - 1);
    int enemyCount = 1;
    
    int enemyHealth = calculateEnemyHealth(currentLevel);
    int enemyDamage = calculateEnemyDamage(currentLevel);
    
    for (int e = 0; e < enemyCount; ++e) {
        int x, y, tries = 0;
        do {
            x = rndX(gen);
            y = rndY(gen);
            tries++;
            if (tries > 1000) break;
        } while (!field->isPositionEmpty(x, y));
        field->getEnemies().emplace_back(enemyHealth, enemyDamage);
        field->getEnemies().back().setPosition(x, y);
        field->getCells()[y][x].setHasEntity(true);
        
        // Уведомляем о создании врага
        EnemySpawnedEvent spawnEvent(x, y, "генерация");
        EventNotifier::getInstance().notify(spawnEvent);
    }
}

