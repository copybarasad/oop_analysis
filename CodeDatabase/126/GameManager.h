#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <iostream>
#include <memory>
#include "../commands/Command.h"

class Game;

template<typename CommandProcessor>
class GameManager {
    CommandProcessor commandProcessor;
    
public:
    GameManager() = default;
    
    void processGameTurn(Game& game) {
        auto command = commandProcessor.getCommand();
        if (command) {
            command->execute(game);
        }
    }
    
    void processLevelTransition(Game& game, GameStatus status) {
        if (status == GameStatus::LEVEL_COMPLETED) {
            std::cout << "Press N for next level or Q to quit: ";
            char choice;
            std::cin >> choice;
            if (choice == 'n' || choice == 'N') {
                NextLevelCommand().execute(game);
            } else {
                QuitCommand().execute(game);
            }
        } else if (status == GameStatus::PLAYER_DIED) {
            std::cout << "Press R to restart or Q to quit: ";
            char choice;
            std::cin >> choice;
            if (choice == 'r' || choice == 'R') {
                RestartCommand().execute(game);
            } else {
                QuitCommand().execute(game);
            }
        }
    }
};

#endif