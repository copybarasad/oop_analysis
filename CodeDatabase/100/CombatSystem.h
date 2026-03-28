#pragma once
#include "GameState.h"
#include "../game_objects/Player.h"
#include "../game_objects/Enemy.h"
#include "../game_objects/EnemyBuilding.h"
#include "../game_objects/Ally.h"
#include <string>

class CombatSystem {
private:
    GameState& gameState;

    // Атаковать
    template<typename T>
    void performAttack(T* target, const std::string& targetName, int attackScore, int deathScore, const std::string& deathMessage); 

    // Проверка на дальнюю атаку
    template<typename T>
    bool checkRangedAttack(T* target, const std::string& targetName, int attachScore, int deathScore, const std::string& deathMessage);

    // Атака союзника
    template<typename T>
    void handleAllyAttack(T* target, const std::string& targetName, const std::string& deathMessage, Ally* ally);

    // Передвижение врагов
    void processGameLogicEnemyMove(Character* enemy);   // Логика хода врага
    void moveSingleEnemy(Character* enemy);             // Передвижение одного врага
    bool isEnemyCellOccupied(Character* enemy) const;   // Проверка не занята ли клетка для хода врага
    void handleEnemyAttack(Character* enemy);           // Логирование атаки врага
    void handleEnemyMovementResult(Character* enemy);   // Логирование хода врага

    // Срабатывание ловушки на враге
    void checkTrapAndApplyDamage(Character* enemy);

    // Передвижение союзников
    void moveSingleAlly(Ally* ally);            // Двигаем каждого союзника
    void processGameLogicAllyMove(Ally* ally);  // Логика зода каждого союзника      
    void handleAllyMovementResult(Ally* ally);  // Логирование хода игрока
    bool isAllyInCell(int x, int y);            // Есть ли союзник в клетке

    // Враг атакует союзника
    void enemyAttackAlly(Character* enemy);

public:
    CombatSystem(GameState& state);

    void processPlayerCombat();           // Логика хода игрока
    void processEnemyMovement();          // Передвижение врагов
    void processAllyMovement();           // Передвижение союзников

    void reset(GameState& newState);
};