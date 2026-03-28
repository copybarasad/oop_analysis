#include "GameManager.h"
#include "Game.h"
#include "InputHandler.h"

int main() {
    Game game;
    GameManager<InputHandler> manager(&game);
    manager.run();
    return 0;
}