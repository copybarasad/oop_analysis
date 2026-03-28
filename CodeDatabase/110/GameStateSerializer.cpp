#include "GameStateSerializer.hpp"

GameStateSerializer::GameStateSerializer (const GameCycleService::GameState& state) :
    state(state) {}

std::string GameStateSerializer::serialize () const {
    std::ostringstream oss;
    
    oss << "currentLevel:" << state.currentLevel << "\n";
    
    switch (state.progress) {
        case GameProgress::PLAYER_DEAD:
        oss << "progress:PLAYER_DEAD\n";
        break;
        case GameProgress::LEVEL_COMPLETED:
        oss << "progress:LEVEL_COMPLETED\n";
        break;
        case GameProgress::ON_GOING:
        oss << "progress:ON_GOING\n";
        break;
    }
    
    return oss.str();
}