#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include <iostream>
#include "icontroller.hpp"
#include "gamemodel.hpp"
#include "enums.hpp"
#include "visualizer.hpp"
#include "consolerenderer.hpp"
#include "logger.hpp"
#include "command_input.hpp"
#include "menustate.hpp"
#include "play_process_state.hpp"
#include "level_complete_state.hpp"
#include "level_transition_state.hpp"
#include "upgrade_character_state.hpp"
#include "gameserializer.hpp"
#include "errors.hpp"
#include "events.hpp"
#include "event_dispatcher.hpp"
#include "loadgamecommand.hpp"
#include "savegamecommand.hpp"

template <typename InputHandler>
class GameController : public IController {
public:
    GameController(Game& game, LogOutput mode);

    void Run();
    void StartNewGame() override;
    void Stop() override;

    bool LoadGame(const std::string& filename) override;
    void SaveGame(const std::string& filename) override;

    void SetState(GameStateType type) override;
    Game& getGame() override;

private:
    Game& game_;
    InputHandler input_;
    Visualizer<ConsoleRenderer> visualizer_;
    Logger logger_;

    bool running_ = false;

    std::unordered_map<GameStateType, std::unique_ptr<ProcessPhase>> states_;
    ProcessPhase* currentState_ = nullptr;

    void InitStates();
};

template <typename InputHandler>
GameController<InputHandler>::GameController(Game& game, LogOutput mode)
    : game_(game),
      input_(),
      visualizer_(game),
      logger_(mode)
{
    InitStates();
    currentState_ = states_[GameStateType::Menu].get();
}


template <typename InputHandler>
void GameController<InputHandler>::InitStates() {
    states_[GameStateType::Menu]            = std::make_unique<MenuState>();
    states_[GameStateType::PlayProcess]     = std::make_unique<PlayProcessState>();
    states_[GameStateType::LevelComplete]   = std::make_unique<LevelCompleteState>();
    states_[GameStateType::LevelTransition] = std::make_unique<LevelTransitionState>();
    states_[GameStateType::Upgrade]         = std::make_unique<UpgradeCharacterState>();
}

template <typename InputHandler>
void GameController<InputHandler>::SetState(GameStateType type) {
    currentState_ = states_[type].get();
    currentState_->OnEnter();
}

template <typename InputHandler>
void GameController<InputHandler>::StartNewGame() {
    running_ = true;
    game_.InitializeWorld();
    EventDispatcher::instance().notify(
        TurnCompletedEvent(0, false)
    );
}

template <typename InputHandler>
void GameController<InputHandler>::Stop() {
    running_ = false;
}

template <typename InputHandler>
Game& GameController<InputHandler>::getGame() {
    return game_;
}

template <typename InputHandler>
void GameController<InputHandler>::Run() {
    running_ = true;
    currentState_->OnEnter();
    while (running_) {
        if (currentState_ == states_[GameStateType::PlayProcess].get()) {
            auto command = input_.ReadCommand();
            if (command) {
                bool turn_done = command->Execute(game_);
                if (turn_done) {
                    game_.NextTurn();
                }
                else {
                    this->Stop();
                    continue;
                }
            }
        }
        currentState_->Update(*this);
    }
}

template <typename InputHandler>
bool GameController<InputHandler>::LoadGame(const std::string& filename) {
    LoadGameCommand cmd(filename);
    return cmd.Execute(game_);
}

template <typename InputHandler>
void GameController<InputHandler>::SaveGame(const std::string& filename) {
    SaveGameCommand cmd(filename);
    cmd.Execute(game_);
}
