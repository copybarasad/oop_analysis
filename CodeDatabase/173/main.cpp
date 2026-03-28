#include "gameloop/gameloop.hpp"

int main() {
    GameLoop *game = new GameLoop();

    game->run();

    return 0;
}
