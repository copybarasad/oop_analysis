#ifndef CONSOLE_RENDERER_H
#define CONSOLE_RENDERER_H

class GameState;

class ConsoleRenderer {
public:
    void render(const GameState& game_state);
};

#endif