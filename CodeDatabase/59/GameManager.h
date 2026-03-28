#pragma once

template <typename InputHandlerT>
class GameManager {
private:
    class Game& game; 
    InputHandlerT inputHandler;

public:
    GameManager(class Game& gameInstance) : game(gameInstance) {}

    void update() {
        if (!game.isRunning()) return;

        auto command = inputHandler.getCommand();
        if (command) {
            command->execute(game);
        }

        if (game.isRunning()) {
             game.updateWorld();
        }
    }
};
