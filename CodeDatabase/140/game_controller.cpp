#include "game_controller.h"
#include "game.h"
#include "commands.h"
#include "keyboard_input.h"

template class GameController<KeyboardInput>;

template <typename TInput>
void GameController<TInput>::step() {
    GameCommand gc = inputProvider.get_next_command();
    handle_command(gc);
}

template <typename TInput>
void GameController<TInput>::run_loop() {
    bool& running = command.get_running();
    command.display_help();

    while (running && !game.get_field()->is_game_over()) {
        handle_command(inputProvider.get_next_command());
    }
}

template <typename TInput>
void GameController<TInput>::handle_command(const GameCommand& gc) {
    switch (gc.type) {
        case CommandType::Quit:
            command.get_running() = false;
            break;
        case CommandType::Help:
            command.display_help();
            break;
        case CommandType::Move:
            command.move_command(gc.direction);
            break;
        case CommandType::SwitchMode:
            command.mode_switch();
            break;
        case CommandType::Attack:
            command.attack(gc.x, gc.y);
            break;
        case CommandType::UseSpell:
            command.use_spell(gc.spellIndex, gc.x, gc.y);
            break;
        case CommandType::Save:
            command.save_game();
            break;
        case CommandType::Load:
            command.load_game();
            break;
        case CommandType::None:
        default:
            break;
    }
}