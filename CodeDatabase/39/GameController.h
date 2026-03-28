#pragma once

#include <utility>
#include "Command.h"
#include "Game.h"

template<typename InputReader>
class GameController {
public:
    GameController(Game& game, InputReader input)
        : game_(game), input_reader_(std::move(input)) {}

    template<typename View>
    void Run(View& view) {
        bool running = true;

        // основной игровой цикл
        while (running && game_.IsPlayerAlive() && !game_.IsQuitRequested()) {
            Command cmd = input_reader_.ReadCommand();
            HandleCommand(cmd, running);

            // после хода игрока выполняем ход союзников / врагов / баз / башен
            game_.ProcessNonPlayerTurn();

            view.Render(game_);
        }
    }

private:
    Game& game_;
    InputReader input_reader_;

    void HandleCommand(const Command& cmd, bool& running) {
        switch (cmd.type) {
            case CommandType::MoveUp:
                game_.MovePlayerUp();
                break;
            case CommandType::MoveDown:
                game_.MovePlayerDown();
                break;
            case CommandType::MoveLeft:
                game_.MovePlayerLeft();
                break;
            case CommandType::MoveRight:
                game_.MovePlayerRight();
                break;
            case CommandType::SwitchMode:
                game_.PublicSwitchPlayerMode();
                break;
            case CommandType::RangedAttack:
                game_.PublicPlayerRangedAttack();
                break;
            case CommandType::CastSpell:
                game_.PublicPlayerCastSpell();
                break;
            case CommandType::Save:
                game_.SaveToFile("save.txt");
                break;
            case CommandType::Load:
                game_.LoadFromFile("save.txt");
                break;
            case CommandType::Quit:
                game_.RequestQuit();
                running = false;
                break;
            case CommandType::None:
            default:
                break;
        }
    }
};
