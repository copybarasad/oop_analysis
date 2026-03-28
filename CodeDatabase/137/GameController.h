#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "Game.h"
#include "Command.h"
#include "GameException.h"
#include <memory>
#include <iostream>


template<typename InputReaderType>
class GameController {
public:
    GameController(InputReaderType& input_reader, Game& game)
        : input_reader_(input_reader),
          game_(game),
          running_(true) {}

    void ProcessInput() {
        if (input_reader_.HasInput()) {
            Command command = input_reader_.ReadInput();
            ExecuteCommand(command);
        }
    }

    void ExecuteCommand(const Command& command) {
        if (!command.IsValid()) return;

        PlayerAction action = command.GetAction();

        switch (action) {
            case PlayerAction::QUIT_GAME:
                running_ = false;
                return;
            case PlayerAction::SAVE_GAME:
                HandleSaveGame();
                return;
            case PlayerAction::LOAD_GAME:
                HandleLoadGame();
                return;
            default:
                break;
        }

        game_.ProcessCommand(command);
    }

    bool IsRunning() const {
        return running_;
    }

    void Stop() {
        running_ = false;
    }

    void SetSaveFilename(const std::string& filename) {
        save_filename_ = filename;
    }

private:
    void HandleSaveGame() {
        try {
            game_.SaveGame(save_filename_);
            std::cout << "Game saved successfully to " << save_filename_ << std::endl;
        } catch (const GameException& e) {
            std::cerr << "Failed to save game: " << e.what() << std::endl;
        }
    }

    void HandleLoadGame() {
        try {
            game_.LoadGame(save_filename_);
            std::cout << "Game loaded successfully from " << save_filename_ << std::endl;
        } catch (const GameException& e) {
            std::cerr << "Failed to load game: " << e.what() << std::endl;
        }
    }

    InputReaderType& input_reader_;
    Game& game_;
    bool running_;
    std::string save_filename_ = "savegame.sav";
};

#endif // GAME_CONTROLLER_H
