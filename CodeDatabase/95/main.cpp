#include "game/game_controller.hpp"
#include "render/cli_render.hpp"
#include "sys/input_handler.hpp"

int main() {
    GameController<TerminalInputHandler, CLIRenderer> controller;
    controller.startGame();
}
