#include "game_controller.h"
#include "console_input_reader.h"
#include "console_render.h"
#include "game_loop.h"
#include "game_view.h"

int main() {
    GameController game(12, 12, 3);
    GameLoop<> loop(game); // использует ConsoleInputReader по умолчанию
    loop.run();
    return 0;
}