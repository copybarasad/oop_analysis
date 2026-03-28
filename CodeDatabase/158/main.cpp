#include "Game/Game.h"

int main() 
{
    Game game;
    game.run();
    GameLogger::close();
    return 0;
}