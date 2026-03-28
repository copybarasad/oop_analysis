#ifndef GAME_STATE_SER_H
#define GAME_STATE_SER_H

#include "../../BaseSerializable/Serializable.hpp"
#include "../../../../GameCycleService/GameCycleService.hpp"

class GameStateSerializer : public Serializable {
    private:
    const GameCycleService::GameState& state;
    
    public:
    GameStateSerializer (const GameCycleService::GameState& state);
    
    std::string serialize () const override;
};

#endif