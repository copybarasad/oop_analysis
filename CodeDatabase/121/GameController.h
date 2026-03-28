#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "LevelController.h"
#include "InputSystem.h"
#include "ActionSystem.h"
#include "RenderSystem.h"
#include "TextSystem.h"
#include "UpgradeSystem.h"
#include "SpellShop.h"
#include "SaveSystem.h"
#include "GameConfig.h"
#include "GameState.h"
#include "GameManager.h"      
#include "GameVisualizer.h"   
#include <memory>

class GameController {
private:
    // Базовые конфигурации и системы
    GameConfig gameConfig;                    // Конфигурация игры
    std::shared_ptr<TextSystem> textSystem;   // Система текста
    
    // Системы ввода, действий и отображения (создаются первыми)
    InputSystem inputSystem;                  // Система ввода
    ActionSystem actionSystem;                // Система действий игрока
    RenderSystem renderSystem;                // Система отображения
    
    // ШАБЛОННЫЕ КЛАССЫ ДЛЯ ЛАБЫ 4 (создаются после их зависимостей)
    GameManager<InputSystem> gameManager;     // Шаблонный менеджер управления
    GameVisualizer<RenderSystem> gameVisualizer; // Шаблонный визуализатор
    
    // Состояние игры и уровни
    std::unique_ptr<LevelController> currentLevel; // Текущий уровень
    UpgradeSystem upgradeSystem;              // Система улучшений
    SpellShop spellShop;                      // Магазин заклинаний
    SaveSystem saveSystem;                    // Система сохранения
    
    // Игровое состояние
    GameState currentState;                   // Текущее состояние игры
    int currentLevelNumber;                   // Номер текущего уровня
    int turnCount;                            // Счетчик ходов
    bool gameRunning;                         // Флаг работы игры

public:
    // Конструктор (правильный порядок инициализации важен!)
    GameController();
    
    // Геттеры для доступа к состоянию игры
    int getCurrentLevelNumber() const { return currentLevelNumber; }
    GameState getCurrentState() const { return currentState; }
    int getTurnCount() const { return turnCount; }
    LevelController* getCurrentLevel() { return currentLevel.get(); }
    const LevelController* getCurrentLevel() const { return currentLevel.get(); }
    const GameConfig& getGameConfig() const { return gameConfig; }
    
    // Сеттеры для изменения состояния (используются системой сохранения)
    void setCurrentLevelNumber(int level) { currentLevelNumber = level; }
    void setCurrentState(GameState state) { currentState = state; }
    void setTurnCount(int turns) { turnCount = turns; }
    void setCurrentLevel(std::unique_ptr<LevelController> level) { currentLevel = std::move(level); }
    
    // Главный метод запуска игры
    void run();

private:
    // Методы управления состояниями игры
    void mainMenu();                          // Главное меню
    void startNewGame();                      // Начать новую игру
    void loadGame();                          // Загрузить игру
    void saveGame();                          // Сохранить игру
    void gameLoop();                          // Главный игровой цикл
    void processPlayerTurn();                 // Обработка хода игрока
    void processAITurn();                     // Обработка ходов ИИ
    void handleLevelComplete();               // Обработка завершения уровня
    void handleGameOver();                    // Обработка проигрыша
    void showUpgradeMenu();                   // Показать меню улучшений
    void showSpellShop();                     // Показать магазин заклинаний
    
    // Обработка ошибок и восстановление
    void handleCriticalError(const std::string& errorMessage); // Критические ошибки
    void createFallbackLevel();               // Создать запасной уровень при ошибках
};

#endif