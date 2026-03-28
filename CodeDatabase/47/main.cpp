#include "game.h"
#include "game_controller.h"
#include "console_input_reader.h"
#include "console_renderer.h"
#include "game_view.h"

int main() {
    Game game;

    GameController<ConsoleInputReader, GameView<ConsoleRenderer>> controller(game);

    controller.run();

    return 0;
}
