#pragma once

#include <cstddef>
#include "pos.h"

namespace game {
    enum class CommandType {
        None,
        Quit,

        Move,
        Attack,
        SwitchMode,

        Save,
        Load,

        CastSpell,

        MenuNewGame,
        MenuLoadGame,
        MenuQuit,

        HelpConfirm,
        HelpOpenSizeMenu,

        SetFieldSize,

        RestartYes,
        RestartNo,

        LevelUp1,
        LevelUp2,
        LevelUp3,

        PopupConfirm // (save / load confirm)
    };

    struct Command {
        CommandType type = CommandType::None;
        Direction   dir  = Direction::None;
        size_t    index  = 0;

        // SetFieldSize
        int w = 0;
        int h = 0;
    };
}