#pragma once
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "logger.h" // Убедись, что файл называется именно так (регистр важен в Linux/Mac, в Windows нет)

class Game {
private:
    int currentLevel;
    bool isGameOver;
    
    // Умные указатели для автоматического управления памятью
    std::unique_ptr<Field> field;
    std::unique_ptr<Player> player;
    
    // Вектор врагов
    std::vector<Enemy> enemies;
    
    // Указатель на логгер
    Logger* logger;

    // Вспомогательный метод
    void processLevelUp();

public:
    Game();
    ~Game() = default;

    // --- Основные методы управления игрой ---
    void setLogger(Logger* newLogger);
    void setupLevel();
    void nextLevel();
    void processEnemyTurn();
    void checkEndConditions();
    
    // --- Методы для Команд (Именно их не хватало!) ---
    // Они возвращают string для вывода результата в консоль
    std::string movePlayer(int dx, int dy);
    std::string playerAttack();
    std::string switchCombatMode();
    std::string useSpell(int index, int targetX, int targetY);
    std::string buySpell();

    // --- Геттеры для отрисовки ---
    const Field* getField() const;
    const Player* getPlayer() const;
    const std::vector<Enemy>& getEnemies() const;
    int getCurrentLevel() const;
    bool isGameRunning() const;
    void setGameOver(bool over);

    // --- Сохранение и загрузка ---
    void saveGame(const std::string& filename);
    void loadGame(const std::string& filename);

    Logger* getLogger() { return logger; }
    
    // Заглушка для совместимости
    void run();
};