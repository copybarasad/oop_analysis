#include "console_input.h"
#include "console_renderer.h"
#include "game.h"
#include "game_controller.h"
#include "game_visualizer.h"

int main() {
    Game game;

    GameVisualizer<ConsoleRenderer> visualizer;
    GameController<ConsoleInput> controller(game, visualizer);

    controller.Run();
    return 0;
}
