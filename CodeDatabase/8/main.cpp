#include "game.h"
#include "game_vizualizator.h"
#include "game_controller.h"
#include "fileinput.h"
using namespace std;

int main() {
    Game game("");
    //Game game("controls.config.txt");
    //GameVisualization<ColorfulRenderer> viz(&game);
    GameVisualization<ConsoleRenderer> viz(&game);
    //game.template startGame<DemoInput>(viz);
    game.template startGame<InputReader>(viz);
    return 0;
}