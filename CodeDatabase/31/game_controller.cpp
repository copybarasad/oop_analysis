#include "game_controller.hpp"
#include "game.hpp"


template<typename InputHandler>
GameController<InputHandler>::GameController() : input_reader() {}

template<typename InputHandler>
Command GameController<InputHandler>::get_next_command() {
        return input_reader.read_input();
    }

template<typename InputHandler>
Command GameController<InputHandler>::get_confirm_command() {
        return Command::Attack;
    }
    
template<typename InputHandler>
bool GameController<InputHandler>::move_cursor(Command command, Point& point, int xmin, int xmax, int ymin, int ymax) {
        return input_reader.move_cursor(command, point, xmin, xmax, ymin, ymax);
    }

template<typename InputHandler>
void GameController<InputHandler>::process_input(Game* game) {
    if (!game) return;
    
    Command cmd = input_reader.read_input();
    
    switch(cmd) {
        case Command::Up:
            game->player_move_up();
            break;
        case Command::Down:
            game->player_move_down();
            break;
        case Command::Left:
            game->player_move_left();
            break;
        case Command::Right:
            game->player_move_right();
            break;
        case Command::Attack:
            game->player_attack();
            break;
        case Command::SwitchAttackType:
            game->player_switch_attack();
            break;
        case Command::ShowHand:
            game->show_hand();
            break;
        case Command::BuySpell:
            game->buy_spell();
            break;
        case Command::UseSpell:
            game->use_spell();
            break;
        case Command::Exit:
            game->exit_game();
            break;
        case Command::Save:
            game->save_game();
            break;
        case Command::Load:
            game->load_game_cmd();
            break;
        case Command::Help:
            game->show_help();
            break;
        default:
            break;
    }
}

template class GameController<InputReader>;