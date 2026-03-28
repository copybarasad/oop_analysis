#include "GameLoop.h"
#include "Game.h"
#include "LevelConfig.h"
#include "SaveManager.h"
#include "Exceptions.h"

#include <iostream>
#include <string>

namespace {
    SaveManager g_saves{"save.txt"};
}

GameLoop::GameLoop(std::unique_ptr<LogSink> sink, const std::string& controls_path)
    : game_(nullptr),
      level_index_(1),
      exit_requested_(false),
      world_loaded_ready_(false),
      save_on_next_level_start_(false),
      bindings_(ControlBindings::LoadOrDefault(controls_path)),
      event_bus_(),
      logger_(std::make_unique<GameLogger>(event_bus_, std::move(sink))),
      controls_path_(controls_path) {}

GameLoop::~GameLoop() = default;

void GameLoop::Run() {
    while (!exit_requested_) {
        if (!game_) {
            ShowStartMenu_();
            continue;
        }

        try {
            RunLevel_();
            if (exit_requested_) break;

            if (game_->LastLevelWasVictory()) {
                PostLevel_();
                world_loaded_ready_ = false;
                continue;
            }

        } catch (const GameError& e) {
            std::cout << "[Error] " << e.what() << "\n";
        } catch (const std::exception& e) {
            std::cout << "[Unexpected] " << e.what() << "\n";
        }
    }
}


void GameLoop::ShowStartMenu_() {
    std::cout << "=== Start ===\n"
              << "[N] New game\n"
              << "[L] Load game\n"
              << "[Q] Quit\n> ";
    std::string s;
    if (!std::getline(std::cin, s)) { exit_requested_ = true; return; }
    if (s.empty()) return;
    switch (std::tolower(static_cast<unsigned char>(s[0]))) {
        case 'n': StartNewGame_(); break;
        case 'l': TryLoadGame_();  break;
        case 'q': exit_requested_ = true; break;
    }
}

void GameLoop::StartNewGame_() {
    std::string name;
    std::cout << "Enter player name: ";
    std::getline(std::cin, name);
    if (name.empty()) name = "Player";

    int cap = 5;
    std::cout << "Hand capacity (default 5): ";
    std::string t; 
    if (std::getline(std::cin, t) && !t.empty()) {
        try { cap = std::max(1, std::stoi(t)); } catch (...) {}
    }

    level_index_ = 1;
    auto cfg = LevelConfig::Make(level_index_);

    game_ = std::make_unique<Game>(cfg, Game::Difficulty::Normal, cap);
    game_->SetEventBus(&event_bus_);
    game_->NewGame(name, cap);
}

void GameLoop::TryLoadGame_() {
    try {
        if (!game_) {
            auto init_cfg = LevelConfig::Make(1);
            game_ = std::make_unique<Game>(init_cfg, Game::Difficulty::Normal, 5);
            game_->SetEventBus(&event_bus_);
        }
        g_saves.Load(*game_);
        level_index_ = game_->CurrentLevelIndex();
        world_loaded_ready_ = true;
        std::cout << "Loaded. Level " << level_index_ << "\n";
    } catch (const GameError& e) {
        std::cout << e.what() << "\n";
    }
}

void GameLoop::RunLevel_() {
    if (!game_) {
        std::cout << "No game started. Creating default profile...\n";
        level_index_ = 1;
        auto cfg = LevelConfig::Make(level_index_);
        game_ = std::make_unique<Game>(cfg, Game::Difficulty::Normal, 5);
        game_->SetEventBus(&event_bus_);
        game_->NewGame("Player", 5);
        game_->ReconfigureFrom(cfg);
    }

    if (!world_loaded_ready_) {
        game_->StartLevel();
        if (save_on_next_level_start_) {
            try { g_saves.Save(*game_); } catch (const GameError& e) { std::cout << e.what() << "\n"; }
            save_on_next_level_start_ = false;
        }
    } else {
        world_loaded_ready_ = false;
    }

    GameVisualizer<ConsoleRenderer> visualizer(ConsoleRenderer(), &event_bus_);
    ConsoleInputReader reader(std::cin, bindings_);
    GameController<ConsoleInputReader, ConsoleRenderer> controller(*game_, std::move(reader), visualizer);

    std::cout << "Controls loaded from " << controls_path_
              << (bindings_.IsUsingDefault() ? " (default used)" : " (custom)") << "\n"
              << "Use :save [file], :load [file] or configured keys.\n";

    auto res = controller.RunLevel();
    if (res == Game::LevelResult::Defeat) {
        GameOver_();
    } else if (res == Game::LevelResult::Quit) {
        exit_requested_ = true;
        return;
    }
}

void GameLoop::PostLevel_() {
    if (!game_->LastLevelWasVictory()) return;

    std::cout << "Upgrade:\n"
                 "1) +Max HP / heal more\n"
                 "2) +Melee damage\n"
                 "3) +Ranged damage\n"
                 "4) +1 Hand capacity\n> ";
    std::string s; std::getline(std::cin, s);
    int ch = (!s.empty() && std::isdigit(static_cast<unsigned char>(s[0]))) ? (s[0]-'0') : 1;
    game_->ApplyUpgradeChoice(ch);
    game_->HealPlayerFull();
    game_->HalveHandRandom();

    ++level_index_;
    game_->SetCurrentLevelIndex(level_index_);
    const auto cfg = LevelConfig::Make(level_index_);
    game_->ReconfigureFrom(cfg);
    save_on_next_level_start_ = true;
    std::cout << "Next level: " << level_index_
              << " (" << cfg.width << "x" << cfg.height
              << ", enemies: " << cfg.enemies
              << ", towers: " << cfg.towers
              << ", turrets: " << cfg.turrets << ")\n";

}

void GameLoop::GameOver_() {
    std::cout << "Defeat.\n";
    if (AskYesNo_("Restart from level 1? [y/n]: ")) {
        StartNewGame_();
        world_loaded_ready_ = false;
    } else {
        exit_requested_ = true;
    }
}

bool GameLoop::AskYesNo_(const std::string& prompt) {
    std::cout << prompt;
    std::string s; std::getline(std::cin, s);
    if (s.empty()) return false;
    return std::tolower(static_cast<unsigned char>(s[0])) == 'y';
}
