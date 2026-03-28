#pragma once

#include <random>
#include <optional>

#include "command.h"
#include "gameconfig.h"
#include "field.h"
#include "player_hand.h"
#include "fight.h"
#include "consoleui.h"

namespace game {
    enum class PlayerTurnResult {
        ExitLevel,      // Exit from level
        NoWorldTurn,    // Skip world turn (return to player turn)
        WorldTurn       // Do world turn
    };

    enum class GameState {
        MainMenu,
        Help,
        Popup,
        SizeMenu,
        RunningLevel,
        RestartPrompt,
        LevelUp,
        Exit
    };

    enum class PopupType {
        None,
        SaveOk,
        SaveError,
        LoadOk,
        LoadError,
        SpellUpgraded
    };

    class ISpell;

    class Game {
        private:
            GameConfig baseCfg_;
            GameConfig cfg_;
            std::mt19937 rng_;
            std::optional<Field> field_;

            GameState state_ = GameState::MainMenu;

            bool exitRequested_ = false;    // exit game flag
            int currentLevel_ = 1;      // current level number

            bool resumeFromSave_ = false;

            // for Player Hand state saving and Fight Mode
            std::optional<PlayerHand> savedHand_;
            std::optional<FightMode>  savedFightMode_;

            PopupType popupType_ = PopupType::None;
            std::string popupText_;
            GameState popupReturnState_ = GameState::MainMenu;

        private:
            void startNewGame();
            void startLevel();
            void prepareConfigForLevel();
            void onLevelWin();
            void openPopup(PopupType type, std::string text, GameState returnState);
            PlayerTurnResult applyInLevel(const Command& cmd);

        public:
            explicit Game(GameConfig cfg, std::mt19937 rng);

            void tick(const Command& cmd);

            // Getters
            bool exitRequested() const { return exitRequested_; }
            GameState state() const { return state_; }

            bool hasField() const { return field_.has_value(); }
            const Field& field() const { return *field_; }

            int currentLevel() const { return currentLevel_; }

            const GameConfig& cfg() const { return cfg_; }
            
            // Popup
            PopupType popupType() const { return popupType_; }
            std::string popupText() const { return popupText_; }

            // Save-load API
            void saveGame(const std::string& filename) const;
            void loadGame(const std::string& filename);
    };
}