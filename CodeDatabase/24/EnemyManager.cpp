#include "EnemyManager.hpp"
#include <algorithm>

// Добавление нового врага на поле
void EnemyManager::addEnemy(std::unique_ptr<Enemy> enemy, const Pos& position) {
    enemies.emplace_back(std::move(enemy), position);
}

// Получение врага по позиции
Enemy* EnemyManager::getEnemyAt(const Pos& position) {
    auto it = std::find_if(enemies.begin(), enemies.end(),
        [&position](const auto& pair) { 
            return pair.second == position && pair.first->isAlive();
        });
    
    return it != enemies.end() ? it->first.get() : nullptr;
}

// Удаление мертвых врагов
void EnemyManager::removeDeadEnemies() {
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const auto& pair) { return !pair.first->isAlive(); }),
        enemies.end());
}

// Получение всех врагов
const std::vector<std::pair<std::unique_ptr<Enemy>, Pos>>& EnemyManager::getEnemies() const {
    return enemies;
}

// Обновление позиции врага
void EnemyManager::updateEnemyPosition(const Pos& oldPos, const Pos& newPos) {
    auto it = std::find_if(enemies.begin(), enemies.end(),
        [&oldPos](const auto& pair) { return pair.second == oldPos; });
    
    if (it != enemies.end()) {
        it->second = newPos;
    }
}

// Логика определения приоритетных направлений
std::vector<Pos> EnemyManager::getDirectionsByPriority(const Pos& enemyPos, const Pos& playerPos) const {
    std::vector<Pos> directions = {
        Pos(1, 0),   // вправо
        Pos(0, 1),   // вниз  
        Pos(-1, 0),  // влево
        Pos(0, -1)   // вверх
    };
    
    // Сортируем направления по расстоянию до игрока
    std::sort(directions.begin(), directions.end(), 
        [&](const Pos& a, const Pos& b) {
            double distA = (enemyPos + a).distanceTo(playerPos);
            double distB = (enemyPos + b).distanceTo(playerPos);
            return distA < distB;
        });
    
    return directions;
}

// Основная логика - вычисление предложений по перемещению
std::vector<EnemyManager::MoveProposal> EnemyManager::calculateMoveProposals(const Pos& playerPos) const {
    std::vector<MoveProposal> proposals;
    
    for (const auto& enemyPair : enemies) {
        const auto& enemy = enemyPair.first;
        const auto& enemyPos = enemyPair.second;
        
        if (!enemy->isAlive()) continue;
        
        // Получаем приоритетные направления движения
        auto directions = getDirectionsByPriority(enemyPos, playerPos);
        
        // Создаем предложение для возможного направления
        for (const auto& direction : directions) {
            Pos newPos = enemyPos + direction;
            
            MoveProposal proposal;
            proposal.enemyCurrentPos = enemyPos;
            proposal.suggestedNewPos = newPos;
            proposal.shouldAttackPlayer = (newPos == playerPos);
            
            proposals.push_back(proposal);
        }
    }
    
    return proposals;
}