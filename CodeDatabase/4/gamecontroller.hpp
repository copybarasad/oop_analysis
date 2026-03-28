#pragma once
#include "command.hpp"
#include "hero.hpp"
#include "level.hpp"

class GameEngine;

template<typename InputReaderType>
class GameController {
    InputReaderType input_reader;
    Hero* player = nullptr;
    Level* level = nullptr;
    GameEngine* engine = nullptr;

public:
    void bind(Hero& p, Level& l, GameEngine& e);
    bool handle_turn();
};