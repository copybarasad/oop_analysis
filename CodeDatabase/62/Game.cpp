#include "Game.h"
#include <SFML/Graphics.hpp>
#include <iostream>

Game::Game(int width, int height, const std::string& loggerType, const std::string& logFile)
    : window_(sf::VideoMode(sf::Vector2u(1280, 720)), "Game")
    , model(width, height, &eventNotifier_)
    , viewManager(model, window_, model)
    , controller(model, events_, window_)
    , clock() {
    window_.setFramerateLimit(60);
    
    // Настраиваем логирование в зависимости от параметра
    if (loggerType == "file") {
        eventNotifier_.addLogger(std::make_unique<FileLogger>(logFile));
    } else if (loggerType == "both") {
        eventNotifier_.addLogger(std::make_unique<ConsoleLogger>());
        eventNotifier_.addLogger(std::make_unique<FileLogger>(logFile));
    } else {
        // По умолчанию - консоль
        eventNotifier_.addLogger(std::make_unique<ConsoleLogger>());
    }
}

void Game::run() {
    while (model.getIsRunning() && window_.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        // Очищаем события перед обработкой
        events_.clear();
        bool shouldClose = controller.processEvents(deltaTime);
        if (shouldClose) {
            window_.close();
        }
        model.update(deltaTime);
        viewManager.render(deltaTime);
    }
    // Закрываем логгеры при завершении
    eventNotifier_.close();
}