#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

class Game;

class InputHandler {
public:
    void handleInput(Game& game);

private:
    void handleNormalInput(Game& game, char input);
    void handleCastingInput(Game& game, char input);
};

#endif