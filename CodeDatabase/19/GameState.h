#pragma once
#include <nlohmann/json.hpp>

enum class GameState {
    PlayerMove,
    AIMove,
    Upgrading,
    Finished
};

NLOHMANN_JSON_SERIALIZE_ENUM(GameState, {
                             {GameState::PlayerMove, "PlayerMove"},
                             {GameState::AIMove, "AIMove"},
                             {GameState::Upgrading, "Upgrading"},
                             {GameState::Finished, "Finished"}
                             });
