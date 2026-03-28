#pragma once

#include "Field.h"
#include "Player.h"
#include "SaveLoadManager.h"
#include "LevelManager.h"
#include "GameVisualize.h"

class Game {
private:
    GameField field;
    Player player;
    int turnCount;
    bool gameRunning;
    SaveLoadManager saveManager;
    LevelManager levelManager;
    ConsoleGameVisualizer visualizer;
    
public:
    Game();
    
    bool processPlayerMove(char direction);  // Только логика движения
    void castSpell(int spellIndex, int targetX, int targetY);  // Каст заклинания
    void switchCombatType();  // Смена типа боя
    void buySpell();  // Покупка заклинания
    void saveGame();  // Сохранение
    void loadGame();  // Загрузка
    
    // Основные методы игры
    void startNewGame(int width, int height);
    void play();  // Без ввода внутри!
    void nextLevel();
    void gameOver();
    
    // Проверки
    bool checkWinCondition() const;
    bool checkLoseCondition() const;
    
    // Отображение
    void display() const;
    
    // Методы для врагов
    void enemyTurn();
    void buildingTurn();
    void processEnemyTurns();
    
    // Геттеры
    Player& getPlayer() { return player; }
    const Player& getPlayer() const { return player; }
    GameField& getField() { return field; }
    const GameField& getField() const { return field; }
    int getTurnCount() const { return turnCount; }
    bool isGameRunning() const { return gameRunning; }
    void setGameRunning(bool running) { gameRunning = running; }
    void showSaveInfo() const;
    void startLevel();
};