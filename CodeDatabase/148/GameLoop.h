#ifndef GAMELOOP_H
#define GAMELOOP_H

#include "GameController.h"
#include "GameView.h"
#include "MenuSystem.h"
#include "Game.h"

/**
 * @brief Класс управления главным игровым циклом
 * 
 * Отвечает за координацию между игровой логикой (Game),
 * вводом (GameController) и отрисовкой (GameView).
 * Разделяет визуализацию и логику.
 */
template<typename InputReaderType, typename RendererType>
class GameLoop {
private:
    Game* game;
    GameController<InputReaderType>* controller;
    GameView<RendererType>* view;
    
public:
    /**
     * @brief Конструктор игрового цикла
     * @param gameInstance Указатель на объект игры
     */
    GameLoop(Game* gameInstance)
        : game(gameInstance),
          controller(new GameController<InputReaderType>(game)),
          view(new GameView<RendererType>(game)) 
    {}
    
    /**
     * @brief Деструктор
     */
    ~GameLoop() {
        delete controller;
        delete view;
    }
    
    // Запрет копирования
    GameLoop(const GameLoop&) = delete;
    GameLoop& operator=(const GameLoop&) = delete;
    
    /**
     * @brief Запустить главный игровой цикл
     * @return true если игра завершена корректно
     */
    bool run();
    
private:
    /**
     * @brief Обработать один игровой ход
     * @return false если нужно выйти из цикла
     */
    bool processTurn();
    
    /**
     * @brief Обработать поражение игрока
     * @return true если игрок хочет начать заново
     */
    bool handleDefeat();
    
    /**
     * @brief Обработать победу на уровне
     * @return true если игрок продолжает играть
     */
    bool handleVictory();
};

// Включаем реализацию для шаблонного класса
#include "GameLoop.cpp"

#endif // GAMELOOP_H


