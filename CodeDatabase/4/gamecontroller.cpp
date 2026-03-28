#include "gamecontroller.hpp"
#include "gameengine.hpp" 
#include "savemanager.hpp"


template<>
void GameController<ConsoleInput>::bind(Hero& p, Level& l, GameEngine& e) {
    player = &p;
    level = &l;
    engine = &e;
}

template<>
bool GameController<ConsoleInput>::handle_turn() {
    if (!player || !level || !engine) return false;

    Command cmd = input_reader.read_command();
    bool turn_spent = false;

    switch (cmd.action) {
        case GameAction::QUIT:
            return false;

        case GameAction::SWAP_WEAPON:
            player->swap_weapon();
            std::cout << "Weapon changed to: " << player->get_weapon_name() << "\n";
            turn_spent = true;
            break;

        case GameAction::BUY_SPELL:
            player->get_spell();
            turn_spent = true;
            break;

        case GameAction::CAST_SPELL:
            if (player->use_spell(cmd.spell_index, level->field, level->roster, cmd.target_x, cmd.target_y)) {
                turn_spent = true;
            }
            break;

        case GameAction::MOVE_UP:
        case GameAction::MOVE_DOWN:
        case GameAction::MOVE_LEFT:
        case GameAction::MOVE_RIGHT: {
            char ch;
            switch (cmd.action) {
                case GameAction::MOVE_UP: ch = 'w'; break;
                case GameAction::MOVE_DOWN: ch = 's'; break;
                case GameAction::MOVE_LEFT: ch = 'a'; break;
                case GameAction::MOVE_RIGHT: ch = 'd'; break;
                default: ch = ' ';
            }
            int result = player->move(level->field.box, level->field.x, level->field.y, ch);
            if (result == -1) return false;
            if (result != 0) {
                level->roster.atack_enemy(player->damage, result);
            }
            turn_spent = true;
            break;
        }
        case GameAction::SAVE:
            engine->save_game(cmd.save_filename + ".sav");
            std::cout << "Game saved to " << cmd.save_filename << ".sav\n";
            return true;

        default:
            std::cout << "Unknown command.\n";
            return true;
    }

    if (turn_spent) {
        level->run_turn(*player);
    }

    return true;
}