#include "console_renderer.hpp"

void ConsoleRenderer::render_world(World* world){
    int size_x = world->get_size_x();
    int size_y = world->get_size_y();
    std::cout << "+";
    for (int x = 0; x < size_x; ++x) {
        std::cout << "--";
    }
    std::cout << "+\n";   

    for(int y = 0; y < size_y; y++){
        std::cout << "|";

        for(int x = 0; x < size_x; x++){
            Cell& cell = world->get_cell(x, y);
            Entity* entity = cell.get_entity();
            char symbol = '.';

            if (!entity){
                switch (cell.get_type_cell()){
                case TypeCell::NORMAL:
                    symbol = '.';
                    break;
                case TypeCell::SLOW:
                    symbol = '~';
                    break;
                case TypeCell::PASSABLE:
                    symbol = '#';
                    break;
                case TypeCell::MAGIC:
                    symbol = '$';
                    break;
                case TypeCell::TRAP:
                    symbol = '*';
                    break;
                }
            } else{
                symbol = entity->get_symbol();
            }

            std::cout << symbol << " ";
        }

        std::cout << "|\n";
    }

    std::cout << "+";
    for (int x = 0; x < size_x; ++x) {
        std::cout << "--";
    }
    std::cout << "+\n";
}


void ConsoleRenderer::render_player_stats(Player& player, int level){
    int hp = player.get_hp();
    int points = player.get_points();
    std::cout << "Player hp: " << hp << " | points: " << points << " | level: " << level << std::endl;
}

void ConsoleRenderer::render_player_hand(Player& player){
    std::vector<ISpellCard*> cards = player.get_hand().get_cards();

    if (cards.empty()){
        std::cout << "your hand is empty" << std::endl;
    }

    int i = 0;
    for (auto card : cards){
        std::cout << "# " << i++ << " card: " << card->get_name() << std::endl;
    }
}

void ConsoleRenderer::render_legend(){
    std::cout << "legend:\nP - Player\nE - Enemy" << std::endl;
    std::cout << ". - Empty cell\n~ - Slow cell\n# - Not Passable\nB - Enemy Build\n$ - magic card\n* - trap for enemy" << std::endl;
    std::cout << "T - Enemy Tower\n" << std::endl;
}

void ConsoleRenderer::render_help(std::unordered_map<std::string, CommandType> keybindings){
    render_legend();
    render_bind_keys(keybindings);
}

void ConsoleRenderer::render_full_ui(Game& game){
    render_legend();
    std::cout << std::endl;
    render_player_stats(game.get_world()->get_player(), game.get_level());
    render_world(game.get_world());
}

void ConsoleRenderer::render_msg(std::string msg){
    std::cout << msg << std::endl;
}

void ConsoleRenderer::render_final_output(bool is_win, int points){
    std::string win_or_lose = "WIN";
    if (!is_win){
        win_or_lose = "Lose";
    }
    std::cout << "You: " << win_or_lose << "\n" << " Your score: " << points << std::endl;
}

void ConsoleRenderer::render_bind_keys(std::unordered_map<std::string, CommandType>& keybindings){
    std::cout << "\n=== Current Keybindings ===" << std::endl;
    std::unordered_map<CommandType, std::string> cmd_to_key;
    for (const auto& pair : keybindings) {
        cmd_to_key[pair.second] = pair.first;
    }

    std::vector<std::pair<std::string, CommandType>> commands = {
        {"Move up", CommandType::MOVE_UP},
        {"Move left", CommandType::MOVE_LEFT},
        {"Move down", CommandType::MOVE_DOWN},
        {"Move right", CommandType::MOVE_RIGHT},
        {"Change attack", CommandType::CHANGE_ATTACK},
        {"Show cards", CommandType::SHOW_CARDS},
        {"Use card", CommandType::USE_CARD},
        {"Help", CommandType::HELP},
        {"End game", CommandType::END_GAME},
        {"Load game", CommandType::LOAD},
        {"Save game", CommandType::SAVE},
        {"Bind keys", CommandType::KEYBIND_SETUP},
    };

    for (auto cmd_pair : commands){
        std::string key = cmd_to_key.count(cmd_pair.second) > 0 ? cmd_to_key[cmd_pair.second] : "not set";
        std::cout << cmd_pair.first << ": " << key << std::endl;
    }
    std::cout << std::endl;
}