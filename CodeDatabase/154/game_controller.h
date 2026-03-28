#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "command.h"
#include "input_handler.h"
#include "field.h"
#include "player.h"
#include <iostream>

template<typename InputHandler>
class GameController {
private:
    InputHandler inputHandler_;
    GameManager* gameManager_;

public:
    GameController(GameManager* gm) : gameManager_(gm) {}

    bool handleCommand(const Command& cmd) {
        if (!cmd.isValid() || !gameManager_) {
            return false;
        }

        switch (cmd.getType()) {
            case CommandType::MoveUp:
                gameManager_->movePlayer(Direction::Up);
                return true;

            case CommandType::MoveDown:
                gameManager_->movePlayer(Direction::Down);
                return true;

            case CommandType::MoveLeft:
                gameManager_->movePlayer(Direction::Left);
                return true;

            case CommandType::MoveRight:
                gameManager_->movePlayer(Direction::Right);
                return true;

            case CommandType::Attack:
                gameManager_->playerAttack();
                return true;

            case CommandType::CastSpell:
                
                gameManager_->playerCastSpell(0);
                return true;

            case CommandType::SaveGame: {
                std::cout << "Enter save filename (default 'savegame.sav'): ";
                std::string filename;
                std::cin.ignore();
                std::getline(std::cin, filename);
                if (filename.empty()) filename = "savegame.sav";
                gameManager_->saveGame(filename);
                return true;
            }

            case CommandType::LoadGame: {
                std::cout << "Enter save filename (default 'savegame.sav'): ";
                std::string filename;
                std::cin.ignore();
                std::getline(std::cin, filename);
                if (filename.empty()) filename = "savegame.sav";
                gameManager_->loadGame(filename);
                return true;
            }

            case CommandType::ShowInfo:
                gameManager_->displayGameInfo();
                return true;

            case CommandType::Quit:
                return false;  

            default:
                return false;
        }
    }

    Command getNextCommand() {
        return inputHandler_.getCommand();
    }

    bool setKeyBindings(const std::map<char, CommandType>& bindings) {
        return inputHandler_.setKeyBindings(bindings);
    }

    bool loadInputConfig(const std::string& configFile) {
        return inputHandler_.loadFromFile(configFile);
    }
};

#endif
