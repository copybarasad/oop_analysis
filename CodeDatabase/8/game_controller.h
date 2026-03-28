#pragma once
#include "command.h"
#include "game.h"
#include "game_vizualizator.h"
#include "concrete_command.h"
#include <iostream>
#include <type_traits>
#include <string>

using namespace std;

template<typename InputProvider, typename RendererType>
class GameController {
private:
    InputProvider inputProvider;
    string lastCommand;
    
    static_assert(
        std::is_same_v<decltype(std::declval<InputProvider>().template readCommand<RendererType>()), Command*>,
        "InputProvider must have method: Command* readCommand<RendererType>()"
    );

public:
    GameController() = default;
    
    explicit GameController(const InputProvider& provider) 
        : inputProvider(provider), lastCommand("") {}
        
    string getLast() const { 
        return lastCommand; 
    }
    bool processInput(Game& game, GameVisualization<RendererType>& visualization) {
        visualization.showPrompt("Введите команду: ");
        try {
            Command* command = inputProvider.template readCommand<RendererType>();
            if (!command) {
                visualization.renderMessage("Неверная команда!");
                return false;
            }
            string resultMessage;
            bool success = false;
            string commandType = command->getType();
            if (commandType == "USE_SPELL") {
                success = static_cast<UseSpellCommand*>(command)
                    ->template executeWithVisualization<RendererType>(game, visualization, resultMessage);
            }
            else if (commandType == "BUY_SPELL") {
                success = static_cast<BuySpellCommand*>(command)
                    ->template executeWithVisualization<RendererType>(game, visualization, resultMessage);
            }
            else if (commandType == "CHANGE_TYPE") {
                success = static_cast<ChangeCombatTypeCommand*>(command)
                    ->template executeWithVisualization<RendererType>(game, visualization, resultMessage);
            }
            else if (commandType == "SHOW_STATUS") {
                success = static_cast<ShowStatusCommand*>(command)
                    ->template executeWithVisualization<RendererType>(game, visualization, resultMessage);
            }
            else if (commandType == "SAVE") {
                success = static_cast<SaveCommand*>(command)
                    ->template executeWithVisualization<RendererType>(game, visualization, resultMessage);
            }
            else {
                success = command->execute(game, resultMessage);
            }
            if (!resultMessage.empty()) {
                visualization.renderMessage(resultMessage);
            }

            lastCommand = commandType;
            delete command;
            return success;
        
        } catch (const exception& e) {
            visualization.renderMessage("Ошибка: " + string(e.what()));
            return false;
        }
    }
    bool processInputUntilSuccess(Game& game, GameVisualization<RendererType>& visualization) {
        int attempts = 0;
        const int MAX_ATTEMPTS = 3;
        
        while (attempts < MAX_ATTEMPTS) {
            bool success = processInput(game, visualization);
            if (success) { 
                return true; 
            }
            
            attempts++;
            if (attempts < MAX_ATTEMPTS) {
                visualization.renderMessage("Неверная команда, попробуйте снова (попытка " + to_string(attempts) + " из " + to_string(MAX_ATTEMPTS) + ")");
            } else {
                visualization.renderMessage("Слишком много неудачных попыток. Пропускаем ход.");
                lastCommand = "";
                return true;
            }
        }
        return false;
    }
    
    InputProvider& getInputProvider() { 
        return inputProvider; 
    }
    
    const InputProvider& getInputProvider() const { 
        return inputProvider; 
    }
    
    virtual ~GameController() = default;
};