#pragma once
#include "Game.h"
#include "Command.h"

template <class T_Input>
class GameController {
private:
    Game& game;
    T_Input input; 

public:
    GameController(Game& g) : game(g) {}

    void run() {
        bool running = true;
        game.refresh_observer();

        while (running && !game.is_finished_state()) {
            Command cmd = input.get_command();
            bool turn_made = false;

            switch (cmd.type) {
                case CommandType::Quit:
                    running = false;
                    break;
                case CommandType::MoveUp:
                    turn_made = game.move_player(0, -1);
                    break;
                case CommandType::MoveDown:
                    turn_made = game.move_player(0, 1);
                    break;
                case CommandType::MoveLeft:
                    turn_made = game.move_player(-1, 0);
                    break;
                case CommandType::MoveRight:
                    turn_made = game.move_player(1, 0);
                    break;
                case CommandType::SwitchWeapon:
                    game.toggle_weapon();
                    turn_made = true;
                    break;
                case CommandType::SaveGame: {
                    std::string name = input.get_string("Введите название сохранения: ");
                    game.save_game_request(name);
                    break;
                }
                case CommandType::CastSpell: {
                    if (game.get_player_spell_count() == 0) {
                         game.log("Нет заклинаний.");
                         break;
                    }
                    game.print_spells_info();
                    int idx = input.get_int("Выберите заклинание (0 - отмена): ");
                    if (idx > 0 && idx <= game.get_player_spell_count()) {
                         int x = input.get_int("Введите координату X: ");
                         int y = input.get_int("Введите координату Y: ");
                         if (game.cast_spell(idx - 1, x, y)) {
                             turn_made = true;
                         }
                    }
                    break;
                }
                case CommandType::OpenShop: {
                    game.print_shop_info();
                    int idx = input.get_int("Выберите товар для покупки (0 - отмена): ");
                    if (idx > 0) {
                        game.buy_item(idx - 1);
                    }
                    break;
                }
                default:
                    game.log("Неизвестная команда.");
                    break;
            }

            if (turn_made) {
                game.update_ai();
                game.refresh_observer();
            }

            if (game.is_victory_state()) {
                running = false;
            }
        }
    }
};
