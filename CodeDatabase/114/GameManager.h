#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Game.h"
#include "SettingsController.h"
#include "../models/commands/Command.h"
#include <memory>
#include <iostream>
#include "input/MovementStrategy.h"
#include "input/CombatModeStrategy.h"
#include "input/AttackStrategy.h"
#include "input/SpellCastStrategy.h"
#include "input/SpellPurchaseStrategy.h"
#include "input/SaveStrategy.h"
#include "input/LoadStrategy.h"
#include "input/HelpStrategy.h"
#include "../models/events/EventNotifier.h"
#include "../models/events/RenderFieldEvent.h"
#include "../models/events/PrintMessageEvent.h"

// Шаблонный класс управления игрой
// Параметр шаблона: класс для чтения ввода (статический полиморфизм)
template<typename InputReader>
class GameManager {
private:
    Game* game;
    InputReader inputController;
    SettingsController* settingsController;
    
    // Обработка команды через создание стратегии
    bool processCommand(CommandType commandType);
    std::unique_ptr<IInputStrategy> createStrategy(CommandType commandType);
    
    // Методы игрового цикла
    void renderGameState();
    void processEnemyTurn();
    void processPetTurn();
    void processBuildingTurn();
    bool isGameOver() const;
    bool isLevelComplete() const;
    
public:
    // Конструктор принимает Game по указателю - НЕ создает его!
    explicit GameManager(Game* game, SettingsController* settingsController = nullptr);
    ~GameManager() = default;
    
    // Запуск игры
    void startGame();
    
    // Выполнение игрового цикла
    void runGameLoop();
    
    // Установка контроллера настроек
    void setSettingsController(SettingsController* controller) {
        settingsController = controller;
        if (settingsController) {
            inputController.setSettingsController(settingsController);
        }
    }
};

// Реализация конструктора
template<typename InputReaderType>
GameManager<InputReaderType>::GameManager(Game* game, SettingsController* settingsController)
    : game(game), settingsController(settingsController)
{
    this->inputController.setSettingsController(settingsController);
    if (!game) {
        throw std::invalid_argument("Game cannot be null");
    }
}

// Реализация startGame
template<typename InputReaderType>
void GameManager<InputReaderType>::startGame() {
    if (!game) {
        return;
    }
    
    // Инициализация игры происходит в Game (обрабатывает выбор: новая игра или загрузка)
    game->startGame();
    
    // Запускаем игровой цикл
    runGameLoop();
}

// Реализация runGameLoop
template<typename InputReaderType>
void GameManager<InputReaderType>::runGameLoop() {
    while (!isGameOver()) {
        renderGameState();
        
        // Читаем команду от пользователя
        CommandType command = inputController.readCommand();
        
        if (command == CommandType::QUIT) {
            break;
        }
        
        // Обрабатываем команду через стратегию
        bool success = processCommand(command);
        
        if (!success) {
            // Игра была перезапущена (например, после загрузки)
            continue;
        }
        
        // Проверяем, не погиб ли игрок после команды
        if (isGameOver()) {
            PrintMessageEvent gameOverEvent("Игрок погиб! Игра окончена.");
            EventNotifier::getInstance().notify(gameOverEvent);
            std::string restart = inputController.getInput("Начать заново? (y/n): ");
            if (restart == "y" || restart == "Y") {
                game->restartGame();
                continue;
            } else {
                break;
            }
        }
        
        // Удаляем мертвых врагов после действий игрока
        game->getField().removeDeadEntities();
        
        // Проверяем завершение уровня после применения заклинаний
        if (isLevelComplete()) {
            PrintMessageEvent victoryEvent("\n=== ПОБЕДА! Все враги уничтожены ===");
            EventNotifier::getInstance().notify(victoryEvent);
            PrintMessageEvent nextLevelEvent("Переход на следующий уровень...");
            EventNotifier::getInstance().notify(nextLevelEvent);
            bool upgradeHealth = false;
            bool upgradeDamage = false;
            inputController.processPlayerUpgrade(upgradeHealth, upgradeDamage);
            game->nextLevel(upgradeHealth, upgradeDamage);
            continue;
        }
        
        // Ход врагов, союзников и зданий
        processEnemyTurn();
        processPetTurn();
        processBuildingTurn();
        
        // Удаляем мертвых врагов после хода врагов
        game->getField().removeDeadEntities();
        
        // Проверяем завершение уровня
        if (isLevelComplete()) {
            PrintMessageEvent victoryEvent("\n=== ПОБЕДА! Все враги уничтожены ===");
            EventNotifier::getInstance().notify(victoryEvent);
            PrintMessageEvent nextLevelEvent("Переход на следующий уровень...");
            EventNotifier::getInstance().notify(nextLevelEvent);
            bool upgradeHealth = false;
            bool upgradeDamage = false;
            inputController.processPlayerUpgrade(upgradeHealth, upgradeDamage);
            game->nextLevel(upgradeHealth, upgradeDamage);
            continue;
        }
        
        // Проверяем, не закончилась ли игра
        if (isGameOver()) {
            break;
        }
    }
}

// Реализация processCommand
template<typename InputReaderType>
bool GameManager<InputReaderType>::processCommand(CommandType commandType) {
    auto strategy = createStrategy(commandType);
    if (strategy) {
        return strategy->handle();
    } else if (commandType == CommandType::UNKNOWN) {
        PrintMessageEvent unknownCommandEvent("Неверная команда!");
        EventNotifier::getInstance().notify(unknownCommandEvent);
        return true;
    }
    return true;
}

// Реализация createStrategy
template<typename InputReaderType>
std::unique_ptr<IInputStrategy> GameManager<InputReaderType>::createStrategy(CommandType commandType) {
    auto* playerController = &(game->getPlayerController());
    auto* handController = &(game->getHandController());
    
    switch (commandType) {
        case CommandType::MOVE_UP:
            return std::make_unique<MovementStrategy>(playerController, 'w');
        case CommandType::MOVE_DOWN:
            return std::make_unique<MovementStrategy>(playerController, 's');
        case CommandType::MOVE_LEFT:
            return std::make_unique<MovementStrategy>(playerController, 'a');
        case CommandType::MOVE_RIGHT:
            return std::make_unique<MovementStrategy>(playerController, 'd');
        case CommandType::SWITCH_COMBAT_MODE:
            return std::make_unique<CombatModeStrategy>(playerController);
        case CommandType::ATTACK:
            return std::make_unique<AttackStrategy>(playerController, &inputController);
        case CommandType::CAST_SPELL:
            return std::make_unique<SpellCastStrategy>(playerController, handController, &inputController);
        case CommandType::PURCHASE_SPELL:
            return std::make_unique<SpellPurchaseStrategy>(playerController, handController, &inputController);
        case CommandType::SAVE_GAME:
            return std::make_unique<SaveStrategy>(game, &inputController);
        case CommandType::LOAD_GAME:
            return std::make_unique<LoadStrategy>(game, &inputController);
        case CommandType::HELP:
            return std::make_unique<HelpStrategy>();
        default:
            return nullptr;
    }
}

// Реализация renderGameState
template<typename InputReaderType>
void GameManager<InputReaderType>::renderGameState() {
    auto& field = game->getField();
    
    // Отправляем событие для отрисовки поля
    RenderFieldEvent renderEvent(&field);
    EventNotifier::getInstance().notify(renderEvent);
    
    // Отправляем событие для вывода уровня
    PrintMessageEvent levelEvent("Текущий уровень: " + std::to_string(game->getCurrentLevel()));
    EventNotifier::getInstance().notify(levelEvent);
}

// Реализация processEnemyTurn
template<typename InputReaderType>
void GameManager<InputReaderType>::processEnemyTurn() {
    auto* enemyController = &(game->getEnemyController());
    if (enemyController) {
        PrintMessageEvent enemyTurnEvent("\n=== ХОД ВРАГОВ ===");
        EventNotifier::getInstance().notify(enemyTurnEvent);
        enemyController->processEnemyTurn();
    }
}

// Реализация processPetTurn
template<typename InputReaderType>
void GameManager<InputReaderType>::processPetTurn() {
    auto* petController = &(game->getPetController());
    if (petController) {
        PrintMessageEvent petTurnEvent("\n=== ХОД СОЮЗНИКОВ ===");
        EventNotifier::getInstance().notify(petTurnEvent);
        petController->processPetTurn();
    }
}

// Реализация processBuildingTurn
template<typename InputReaderType>
void GameManager<InputReaderType>::processBuildingTurn() {
    auto* buildingController = &(game->getBuildingController());
    if (buildingController) {
        PrintMessageEvent buildingTurnEvent("\n=== ХОД ЗДАНИЙ ===");
        EventNotifier::getInstance().notify(buildingTurnEvent);
        buildingController->processBuildingTurn();
    }
}

// Реализация isGameOver
template<typename InputReaderType>
bool GameManager<InputReaderType>::isGameOver() const {
    auto* playerController = &(game->getPlayerController());
    return !playerController || !playerController->isAlive();
}

// Реализация isLevelComplete
template<typename InputReaderType>
bool GameManager<InputReaderType>::isLevelComplete() const {
    auto& field = game->getField();
    return field.getEnemies().empty() && field.getBuilding().empty();
}

#endif // GAMEMANAGER_H
