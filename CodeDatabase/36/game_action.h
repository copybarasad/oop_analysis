#ifndef GAME_ACTION_H
#define GAME_ACTION_H

enum class GameAction {
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    Attack,
    CastSpell,
    SwitchMode,
    SaveGame,
    LoadGame,
    Quit,
    None,
    Count
};

#endif