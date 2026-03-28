#include "game.hpp"
#include <cstdlib>
#include <ctime>

int main()
{
    Game game;
    game.init_game();
    game.game_loop();
    return 0;
}