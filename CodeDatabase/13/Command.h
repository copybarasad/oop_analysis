#ifndef GAME_COMMAND_H
#define GAME_COMMAND_H

enum class CommandType {
    None,

    Exit,
    SaveGame,
    LoadGame,
    CloseGame,

    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,

    ArrowLeft,
    ArrowRight,
    ArrowUp,
    ArrowDown,

    ChangeWeapon,
    ToggleBook,
    ToggleSpellInfo,
    CastOrConfirm,
};

#endif
