#include "game_control.h"
#include "console_input.h"
#include "Game.h"
int main() {
    Game* g = new Game(20, 15);
    GameControl<ConsoleInput> gc(g);
    gc.game_on();
    
    return 0;
}