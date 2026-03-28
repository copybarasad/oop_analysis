#include "app.hpp"

App::App(std::string db_name):
    db(db_name), show_menu(true) {}


Game* App::load_game() {
    std::string load_name;

    while (!db.save_exists(load_name = Control::get_save_name())) {}

    GameImage* game_img = nullptr;
    while(game_img == nullptr) {
        game_img = db.load_game(load_name);
    }
    Game* game = new Game(*game_img);

    return game;
}

Game* App::create_game(char com) {
    if (com == Menu::NEW_GAME) {
        Game* game = new Game();
        return game;
    }
    else if (com == Menu::LOAD_GAME) {
        return load_game();
    }
}

void App::run() {
    Control::set_keypress();

    while (true) {
        if (show_menu) {
            char ac = Control::get_menu_action_on_start();
            game = create_game(ac);
            show_menu = false;
        }

        GameState res = game->start();

        if (res == GameState::VICTORY) {
            printf("Вы победили!");
            game->next_lvl();
        }
        else if (res == GameState::DEFEAT) {
            printf("Вы проиграли");
            game->restart_lvl();
        } 
        else {
            char com = Control::get_prev_command();

            if (com == Menu::SAVE_GAME) {
                std::string save_name = Control::get_save_name();
                while(!db.save_game(save_name, game->get_data_image()));
                show_menu = true;
            } else {
                game = create_game(com);
            }
        }
    }

    Control::reset_keypress();
}