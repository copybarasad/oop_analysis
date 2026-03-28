#ifndef TEMPLATE_GAME_MANAGER_H
#define TEMPLATE_GAME_MANAGER_H

#include "GameController.h"
#include "InputHandler.h"
#include "GameVisualizer.h"
#include <iostream>

template<typename TInputHandler, typename TRenderer>
class TemplateGameManager {
private:
    TInputHandler inputHandler;
    GameVisualizer<TRenderer> visualizer;
    GameController* gameController;
    
public:
    TemplateGameManager(GameController* controller)
        : inputHandler(), visualizer(controller), gameController(controller) {}
    
    void runGameLoop() {
        if (!gameController) return;
        
        while (gameController->isGameRunning()) {
            visualizer.render();
            
            std::cout << "\nEnter command (WASD=move, IJKL=attack, M=mode, 1-6=spells, P=purchase, O=save, Q=quit): ";
            
            std::unique_ptr<Command> command = inputHandler.readInput(std::cin);
            
            if (!command) {
                std::cout << "Invalid command!\n";
                continue;
            }
            
            if (InputHandler::isQuitCommand(command.get())) {
                    break;
            }

            command->execute(*gameController);

            gameController->updateGame();
        }
    }
};

#endif

