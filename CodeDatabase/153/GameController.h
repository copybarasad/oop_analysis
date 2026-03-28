#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "GameState.h"
#include "InputHandler.h"
#include "ActionProcessor.h"
#include "Command.h"
#include "ShopSystem.h"
#include "GameSaveSystem.h"
#include "CommandType.h"
#include <memory>
#include <iostream>

template<typename InputHandlerType>
class GameController {
private:
    GameState& gameState;
    ActionProcessor& actionProcessor;
    ShopSystem& shopSystem;
    GameSaveSystem& saveSystem;
    std::unique_ptr<InputHandlerType> inputHandler;
    bool isRunning;

public:
    explicit GameController(GameState& state, ActionProcessor& processor,
        ShopSystem& shop, GameSaveSystem& saveSys)
        : gameState(state),
        actionProcessor(processor),
        shopSystem(shop),
        saveSystem(saveSys),
        inputHandler(std::make_unique<InputHandlerType>()),
        isRunning(true) {
        inputHandler->setShopSystem(shopSystem);
        inputHandler->setSaveSystem(saveSystem);
    }

    void processInput() {
        // Получаем тип команды
        CommandType commandType = inputHandler->getCommandType();

        if (commandType == CommandType::INVALID) {
            std::cout << "Invalid command!" << std::endl;
            return;
        }

        // Создаем объект команды
        auto command = inputHandler->createCommand(commandType);
        if (!command) {
            std::cout << "Failed to create command!" << std::endl;
            return;
        }

        // Выполняем команду
        bool success = command->execute(gameState);
        if (success) {
            std::cout << "Command executed successfully." << std::endl;
        }
        else {
            std::cout << "Command failed!" << std::endl;
        }

        // Проверяем выход
        if (commandType == CommandType::QUIT_GAME) {
            stopGame();
        }
    }

    bool isGameRunning() const {
        return isRunning;
    }

    void stopGame() {
        isRunning = false;
    }
};

#endif 