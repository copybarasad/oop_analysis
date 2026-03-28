#pragma once
#include "Enums.h"
#include "Position.h"
#include <optional>

// Тип действия игрока в свой ход
enum class PlayerAction {
    Move,
    Attack,
    SwitchMode,
    UseSpell,
    Skip
};

// Команда игрока на текущий ход.
// В зависимости от action, используются соответствующие поля.
struct PlayerCommand {
    PlayerAction action{PlayerAction::Skip};

    // Для Move
    Direction direction{Direction::None};

    // Для SwitchMode
    AttackMode mode{AttackMode::Melee};

    // Для UseSpell
    std::optional<Position> target{};
    std::optional<int> spellIndex{}; // индекс слота в руке
};
