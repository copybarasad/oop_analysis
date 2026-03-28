#pragma once
#include <memory>
#include <string>
#include "ControlBindings.h"
#include "GameEventBus.h"
#include "GameLogger.h"
#include "ConsoleRenderer.h"
#include "ConsoleInputReader.h"
#include "GameVisualizer.h"
#include "GameController.h"

class Game;

class GameLoop {
public:
    GameLoop(std::unique_ptr<LogSink> sink, const std::string& controls_path);
    ~GameLoop();
    void Run();

private:
    void ShowStartMenu_();
    void StartNewGame_();
    void TryLoadGame_();
    void RunLevel_();
    void PostLevel_();
    void GameOver_();
    bool AskYesNo_(const std::string& prompt);

private:
    std::unique_ptr<Game> game_;
    int level_index_;
    bool exit_requested_;
    bool world_loaded_ready_;
    bool save_on_next_level_start_; 

    ControlBindings bindings_;
    GameEventBus event_bus_;
    std::unique_ptr<GameLogger> logger_;
    std::string controls_path_;
};
