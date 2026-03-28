#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Player.h"
#include "GameField.h"
#include "LevelManager.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include "Ally.h"
#include "Trap.h"
#include "GameTypes.h"
#include <vector>
#include <memory>
#include <string>

// Структура для сохранения состояния врага
struct EnemyState {
    int x;
    int y;
    int health;
    int damage;
    
    EnemyState() : x(0), y(0), health(0), damage(0) {}
    EnemyState(const Enemy& enemy);
    Enemy ToEnemy() const;
};

// Структура для сохранения состояния здания
struct BuildingState {
    int x;
    int y;
    int spawnInterval;
    int currentCounter;
    int health;
    
    BuildingState() : x(0), y(0), spawnInterval(0), currentCounter(0), health(100) {}
    BuildingState(const EnemyBuilding& building);
    EnemyBuilding ToBuilding() const;
};

// Структура для сохранения состояния башни
struct TowerState {
    int x;
    int y;
    int attackRadius;
    int damage;
    int cooldownTurns;
    int currentCooldown;
    
    TowerState() : x(0), y(0), attackRadius(0), damage(0), cooldownTurns(0), currentCooldown(0) {}
    TowerState(const EnemyTower& tower);
    EnemyTower ToTower() const;
};

// Структура для сохранения состояния союзника
struct AllyState {
    int x;
    int y;
    int health;
    int damage;
    
    AllyState() : x(0), y(0), health(0), damage(0) {}
    AllyState(const Ally& ally);
    Ally ToAlly() const;
};

// Структура для сохранения состояния ловушки
struct TrapState {
    int x;
    int y;
    int damage;
    bool triggered;
    
    TrapState() : x(0), y(0), damage(0), triggered(false) {}
    TrapState(const Trap& trap);
    Trap ToTrap() const;
};

// Структура для сохранения состояния заклинания
struct SpellState {
    std::string type;  // "DirectDamage", "AreaDamage", "Trap", "Summon", "Enhancement"
    int baseDamage;
    int currentDamage;
    int baseRadius;
    int currentRadius;
    int extraData;  // Для AreaDamage (areaSize) или Summon (allyCount)
    
    SpellState() = default;
    SpellState(const Spell* spell);
    std::unique_ptr<Spell> ToSpell() const;
};

// Основной класс состояния игры
class GameState {
public:
    // Состояние игрока
    int playerHealth;
    int playerMaxHealth;
    int playerScore;
    int playerMeleeDamage;
    int playerRangedDamage;
    CombatMode playerCombatMode;
    bool playerIsSlowed;
    std::vector<SpellState> playerSpells;
    
    // Состояние поля
    int fieldWidth;
    int fieldHeight;
    int playerX;
    int playerY;
    std::vector<std::vector<CellType>> fieldCells;
    
    // Состояние сущностей
    std::vector<EnemyState> enemies;
    std::vector<BuildingState> buildings;
    std::vector<TowerState> towers;
    std::vector<AllyState> allies;
    std::vector<TrapState> traps;
    
    // Состояние уровня
    int currentLevel;
    int currentTurn;
    
    // Конструкторы
    GameState() = default;
    GameState(const Player& player, const GameField& field, const LevelManager& levelManager, int turn);
    
    // Применить состояние к игре
    void ApplyToGame(Player& player, GameField& field, LevelManager& levelManager, int& turn) const;
    
    // Статический метод для захвата состояния
    static GameState CaptureFromGame(const Player& player, const GameField& field, 
                                     const LevelManager& levelManager, int turn);
    
    // Сериализация
    void SaveToFile(const std::string& filename) const;
    static GameState LoadFromFile(const std::string& filename);
    
private:
    void capturePlayerState(const Player& player);
    void captureFieldState(const GameField& field);
    void captureLevelState(const LevelManager& levelManager, int turn);
    
    void restorePlayerState(Player& player) const;
    void restoreFieldState(GameField& field) const;
    void restoreLevelState(LevelManager& levelManager, int& turn) const;
};

#endif

