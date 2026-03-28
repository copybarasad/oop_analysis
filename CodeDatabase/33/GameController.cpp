#include "GameController.h"
#include "ConsoleInputReader.h"
#include "ConsoleRenderer.h"
#include <iostream>
#include <utility>

template <typename InputReader, typename Renderer>
GameController<InputReader, Renderer>::GameController(
    Game& game,
    InputReader reader,
    GameVisualizer<Renderer>& visualizer)
    : game_(game),
      reader_(std::move(reader)),
      visualizer_(visualizer) {}

template <typename InputReader, typename Renderer>
Game::LevelResult GameController<InputReader, Renderer>::RunLevel() {
    game_.ResetLevelFlags();
    visualizer_.ForceRender(game_);

    enum class TurnResult { Continue, Victory, Defeat, Quit };
    auto check_state = [&]() -> TurnResult {
        if (game_.QuitRequested()) return TurnResult::Quit;
        if (!game_.GetPlayer().IsAlive()) return TurnResult::Defeat;
        if (game_.Enemies().empty()) {
            game_.MarkVictory();
            return TurnResult::Victory;
        }
        return TurnResult::Continue;
    };

    while (game_.GetPlayer().IsAlive()) {
        std::cout << "> ";
        auto cmd = reader_.ReadCommand();
        CommandState state = HandlePlayerCommand(cmd);
        if (state == CommandState::Invalid) {
            std::cout << "Unknown command. Use configured controls or :save/:load.\n";
            continue;
        }
        if (state == CommandState::SkipTurn) {
            visualizer_.RenderIfNeeded(game_);
            continue;
        }

        game_.CleanupDead();
        game_.MaybeRewardSpell();
        auto state_after_player = check_state();
        if (state_after_player != TurnResult::Continue) {
            visualizer_.RenderIfNeeded(game_);
            if (state_after_player == TurnResult::Quit) return Game::LevelResult::Quit;
            if (state_after_player == TurnResult::Defeat) return Game::LevelResult::Defeat;
            return Game::LevelResult::Victory;
        }

        game_.AlliesTurn();
        game_.CleanupDead();
        game_.MaybeRewardSpell();
        auto state_after_allies = check_state();
        if (state_after_allies != TurnResult::Continue) {
            visualizer_.RenderIfNeeded(game_);
            if (state_after_allies == TurnResult::Quit) return Game::LevelResult::Quit;
            if (state_after_allies == TurnResult::Defeat) return Game::LevelResult::Defeat;
            return Game::LevelResult::Victory;
        }

        game_.EnemiesTurn();
        game_.CleanupDead();
        game_.MaybeRewardSpell();
        auto state_after_enemies = check_state();
        if (state_after_enemies != TurnResult::Continue) {
            visualizer_.RenderIfNeeded(game_);
            if (state_after_enemies == TurnResult::Quit) return Game::LevelResult::Quit;
            if (state_after_enemies == TurnResult::Defeat) return Game::LevelResult::Defeat;
            return Game::LevelResult::Victory;
        }

        game_.TurretsTurn();
        game_.CleanupDead();
        game_.MaybeRewardSpell();
        auto state_after_turrets = check_state();
        if (state_after_turrets != TurnResult::Continue) {
            visualizer_.RenderIfNeeded(game_);
            if (state_after_turrets == TurnResult::Quit) return Game::LevelResult::Quit;
            if (state_after_turrets == TurnResult::Defeat) return Game::LevelResult::Defeat;
            return Game::LevelResult::Victory;
        }

        game_.TrySpawnFromTowers();
        game_.CleanupDead();
        game_.MaybeRewardSpell();
        auto state_after_spawn = check_state();
        if (state_after_spawn != TurnResult::Continue) {
            visualizer_.ForceRender(game_);
            if (state_after_spawn == TurnResult::Quit) return Game::LevelResult::Quit;
            if (state_after_spawn == TurnResult::Defeat) return Game::LevelResult::Defeat;
            return Game::LevelResult::Victory;
        }

        visualizer_.ForceRender(game_);
    }

    return Game::LevelResult::Defeat;
}

template <typename InputReader, typename Renderer>
typename GameController<InputReader, Renderer>::CommandState
GameController<InputReader, Renderer>::HandlePlayerCommand(const Command& cmd) {
    switch (cmd.type) {
        case CommandType::Move:
            game_.TryMovePlayer(cmd.dx, cmd.dy);
            return CommandState::Consumed;
        case CommandType::Attack:
            game_.PlayerAttack(cmd.dx, cmd.dy);
            return CommandState::Consumed;
        case CommandType::ToggleMode:
            game_.ToggleMode();
            return CommandState::Consumed;
        case CommandType::CastSpell:
            if (cmd.spell_index < 0) return CommandState::Invalid;
            return game_.CastSpellByIndex(cmd.spell_index)
                ? CommandState::Consumed
                : CommandState::SkipTurn;
        case CommandType::Save:
            game_.TrySaveCommand(cmd.argument);
            return CommandState::SkipTurn;
        case CommandType::Load:
            game_.TryLoadCommand(cmd.argument);
            visualizer_.ForceRender(game_);
            return CommandState::SkipTurn;
        case CommandType::Quit:
            game_.RequestQuit();
            return CommandState::Consumed;
        default:
            return CommandState::Invalid;
    }
}

template class GameController<ConsoleInputReader, ConsoleRenderer>;
