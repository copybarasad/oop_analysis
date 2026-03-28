#pragma once
    enum class CommandType {
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    ChangeWeapon,
    CastSpellUp,
    CastSpellDown,
    CastSpellLeft,
    CastSpellRight,
    SaveGame,
    LoadGame,
    None,
};


struct Command {


CommandType get_input();




};
