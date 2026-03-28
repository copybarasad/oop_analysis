#ifndef COMMAND_H
#define COMMAND_H

#include "game_enums.h"
#include "position.h"
#include <optional>
#include <string>

enum class CommandType {
    Invalid,

    // Main menu
    StartNewGame,
    OpenLoadMenu,
    ExitProgram,

    // Load menu
    LoadSelectedSave,
    BackToMainMenu,

    // In-game
    Move,
    SwitchMode,
    CastSpell,
    BuySpell,
    OpenInGameMenu,
    QuitGame,

    // In-game menu
    SaveGame,
    ContinueGame,
    ReturnToMainMenu
};

#endif