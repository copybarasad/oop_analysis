#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

template<typename InputHandler>
class GameController {
    InputHandler input;

public:
    explicit GameController(InputHandler handler)
        : input(std::move(handler)) {
    }

    void processInput(GameField& field, Player& player) {
        auto cmd = input.getNextCommand();
        if (cmd) {
            cmd->execute(&field, &player);
        }
    }
};

#endif