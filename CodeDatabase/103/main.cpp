#include "main.h"

int main() {
    GameLogic<ConsoleInputSource, ConsoleRenderer> game(10, 10, 2);
    game.run();
    return 0;
}