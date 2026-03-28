#pragma once


enum class Command {
    MoveUp, MoveDown, MoveLeft, MoveRight,
    AttackMode,
    AttackUp, AttackDown, AttackLeft, AttackRight,
    CastSpell,
    SelectSpell,
    Save,
    Load,
    NewGame,
    Settings,
    Exit,
    Back,
    ImproveHealth,
    ImproveHand,
    None
};