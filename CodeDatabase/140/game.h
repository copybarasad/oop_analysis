#pragma once
#include <memory>
#include "field.h"


class Command;
class Renderer;
class InputHandler;

class Game {
    std::unique_ptr<Field> field;
    bool running;
    std::unique_ptr<Command> command;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<InputHandler> input_handler;

    void start_new_game();
    void game_loop();
    void game_over_screen();
    bool ask_restart_or_exit();
    void advance_to_next_level();
    void show_upgrade_menu();

public:
    Game();
    ~Game();
    void run();
    void save_game() const;
    void load_game();

    Field* get_field() const { return field.get(); }
};