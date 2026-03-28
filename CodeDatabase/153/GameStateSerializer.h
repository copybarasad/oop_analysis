#ifndef GAMESTATESERIALIZER_H
#define GAMESTATESERIALIZER_H

#include "GameState.h"
#include <iostream>

class GameStateSerializer {
public:
    void serialize(const GameState& gameState, std::ostream& stream) const;
    void deserialize(GameState& gameState, std::istream& stream) const;
};

#endif