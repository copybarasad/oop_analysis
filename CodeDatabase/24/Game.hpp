#pragma once
#include "Field.hpp"
#include "InputHandler.hpp"
#include "ConsoleRenderer.hpp"
#include "GameController.hpp"
#include "GameVisualizer.hpp"
#include "SaveManager.hpp"
#include <string>

// Основной класс игры, управляющий игровым процессом и состояниями
// использует шаблонные классы для ввода и вывода
class Game {
public:
    // Возможные состояния игры
    enum class State {
        MAIN_MENU,     // Главное меню
        PLAYING,        // Игровой процесс
        GAME_OVER,      // Поражение
        VICTORY,        // Победа на уровне
        LOAD_MENU,      // Меню загрузки сохранения
        PAUSE_MENU      // Меню паузы
    };

private:
    Field field;                    // Игровое поле
    State currentState;            // Текущее состояние игры
    int currentLevel;              // Текущий уровень
    bool gameRunning;              // Флаг работы игрового цикла
    
    // Шаблонные классы для ввода и вывода
    GameController<InputHandler> controller;
    GameVisualizer<ConsoleRenderer> visualizer;

    // Обработчики для каждого состояния
    void handleMainMenu();
    void handlePlaying();
    void handleGameOver();
    void handleVictory();
    void handlePauseMenu();
    void handleLoadMenu();

    // Вспомогательные методы
    void startNewGame();
    void loadGame();
    void saveGame();
    void nextLevel();

public:
    Game();
    void run();
};
