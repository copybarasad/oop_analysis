#pragma once
#include <vector>
#include <memory>
#include <functional>
#include "Enemy.hpp"
#include "Pos.hpp"

// Класс для управления врагами на поле
class EnemyManager {
private:
    // Вектор пар: враг и его позиция
    std::vector<std::pair<std::unique_ptr<Enemy>, Pos>> enemies;

public:
    struct MoveProposal {
        Pos enemyCurrentPos;
        Pos suggestedNewPos;
        bool shouldAttackPlayer;
    };

    EnemyManager() = default;

    // Добавление нового врага
    void addEnemy(std::unique_ptr<Enemy> enemy, const Pos& position);
    
    // Получение врага по позиции
    Enemy* getEnemyAt(const Pos& position);
    
    // Удаление мертвых врагов
    void removeDeadEnemies();
    
    // Получение всех врагов для их обработки
    const std::vector<std::pair<std::unique_ptr<Enemy>, Pos>>& getEnemies() const;
    
    // Обновление позиции врага
    void updateEnemyPosition(const Pos& oldPos, const Pos& newPos);

    // Основной метод - вычисляет предложения по перемещению
    std::vector<MoveProposal> calculateMoveProposals(const Pos& playerPos) const;
    
    // Вспомогательный метод - логика определения приоритетных направлений
    std::vector<Pos> getDirectionsByPriority(const Pos& enemyPos, const Pos& playerPos) const;
};
