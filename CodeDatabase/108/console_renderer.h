#ifndef OOP_CONSOLE_RENDERER_H
#define OOP_CONSOLE_RENDERER_H
#include "game_state.h"
#include "player.h"

class ConsoleRenderer {
public:
    void draw_field(const Player& player, const GameState& game_state, int current_turn);
};


#endif //OOP_CONSOLE_RENDERER_H