#include "game.h"

int main() {
    Game game;
    game.showMainMenu(); 
    if (game.isGameRunning()) {
        game.runGameLoop(); 
    }
    return 0;
}
