#pragma once

enum class Motion {
    None,
    
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    ChangeForm,

    StartCast1,
    StartCast2,
    StartCast3,
    StartCast4,
    StartCast5,

    MoveUpCast,
    MoveDownCast,
    MoveLeftCast,
    MoveRightCast,
    CancelCast,
    ConfirmCast,

    NewGame,
    LoadSave,
    FinishLevel,
    SaveGame,

    UpgradeHp,
    UpgradeDamage,
    UpgradeTargetS,
    UpgradeAreaS,
    UpgradeTrapS
};

enum class Command {
    None,
    
    Up,
    Left,
    Down,
    Right,
    Change,

    Cancel,
    Confirm,

    One,
    Two,
    Three,
    Four,
    Five,

    New,
    Load,
    Finish,
    Save
};
