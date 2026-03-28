
#pragma once
#include "Command.h"
#include "Game.h"
#include "GameVisualizer.h"
#include "ConsoleRenderer.h"

template <class TInput>
class GameControllerT {
public:
    explicit GameControllerT(Game& game)
        : game_(game), input_() {}

    void Run();

private:
    Game& game_;
    TInput input_;
    GameVisualizer<ConsoleRenderer> visualizer_{};

    void Execute(Command cmd);
};


template <class TInput>
void GameControllerT<TInput>::Run() {
    visualizer_.SetBindings(input_.Bindings());
    while (game_.IsRunning()) {

        visualizer_.Draw(game_);

        Command cmd = input_.GetCommand();
        Execute(cmd);

        game_.UpdateTurn();
        

        if (game_.State() == GameState::LevelCompleted) {
            visualizer_.Draw(game_);
            visualizer_.ShowMessage("=== LEVEL CLEARED ===");
            game_.StartNextLevel();
        }

        if (game_.State() == GameState::GameOver) {
            visualizer_.ShowMessage("=== GAME OVER ===");
            break;
        }
    }
}

template <class TInput>
void GameControllerT<TInput>::Execute(Command cmd) {
    switch (cmd) {
        case Command::MoveUp:    game_.MovePlayer(0, -1); break;
        case Command::MoveDown:  game_.MovePlayer(0,  1); break;
        case Command::MoveLeft:  game_.MovePlayer(-1, 0); break;
        case Command::MoveRight: game_.MovePlayer(1,  0); break;

        case Command::Attack:     game_.PlayerAttack(); break;
        case Command::SwitchMode: game_.PlayerSwitchMode(); break;
        case Command::CastSpell:  game_.PlayerCastSpell(); break;

        case Command::Save:
            SaveManager::Save(game_, "saves/save1.txt");
            break;

        case Command::Quit:
            game_.Stop();
            break;

        default:
            break;
    }
}
