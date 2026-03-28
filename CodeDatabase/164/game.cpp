#include "game.h"

const char* ExitException::what() const noexcept override {
    return "Exit game.";
};

Game::Game(): view(), control() {
    logger.log("Game object created");
};

void Game::start_game(){
    logger.log("start_game called");
    view.print_message("Wellcome to the Game of Kasimova Varvara!");
    view.print_main_menu(control);
    char action;
    while(true){
        action = user_input.input_char();
        if(action == control.start || action == control.load) break;
        if(action == control.exit) throw ExitException();
    }

    if(action == control.start){
        init_game();
    }else{
        load_game();
        }
    view.print_message("Let's play!");
};

void Game::main_loop(){
    Hero* hero = static_cast<Hero*>(field.cells[field.hero_coord[1]][field.hero_coord[0]].occupied);
    while(true){
        Hero_turn();
        Ally_turn();
        Enemy_turn();
        if(!(field.cells[field.hero_coord[1]][field.hero_coord[0]].occupied)){
            end_game(end_type::defeat);
        }
        Base_turn();
        Tower_turn();
        if(!(field.cells[field.hero_coord[1]][field.hero_coord[0]].occupied)){
            end_game(end_type::defeat);
        }
    }
};

void Game::init_game(){
    logger.log("init_game called");
    unsigned int level;
    view.print_message("Please, enter the level of Game:");
    view.print_message("\t1 - easy\n\t2 - normal\n\t3 - hard");
    level = user_input.input_int();
    if(level <= 1){
        hard_lvl = 1;
        view.print_message("Your Game level is esay");
    }else if(level >= 3){ 
            hard_lvl = 3;
        view.print_message("Your Game level is hard");
    }else{
        hard_lvl = 2;
        view.print_message("Your Game level is normal");
    }
    game_lvl = 1;
    unsigned int lvl = game_lvl + hard_lvl - 1;
    win_points = 25 * ((7 * lvl - 3) / 4);

    unsigned int height, width;
    view.print_message("Please, enter height and width of field:");
    height = user_input.input_int();
    width = user_input.input_int();
    field = Field(height, width, game_lvl, hard_lvl);
    hand = Hand(game_lvl);
};

void Game::load_game() {
    logger.log("load_game called");
    char input;
    std::ifstream file(SAVE_GAME_FILE);
    if (!file.is_open()) {
        view.print_message("No saved game found!");
        view.print_new_exit(control);
        while(true){
            input = user_input.input_char();
            if(input == control.yes) break;
            if(input == control.exit) throw ExitException();
        }
        init_game();
    }else{
        try{
            json game_data = json::parse(file);
                
            game_lvl = game_data["game_lvl"];
            hard_lvl = game_data["hard_lvl"];
            win_points = game_data["win_points"];
                
            field.from_json(game_data["field"], game_lvl, hard_lvl);
            hand.from_json(game_data["hand"], game_lvl);            
            view.print_message("Game loaded successfully!");
                
        } catch(const std::exception& e) {
            view.print_message("Error loading game:");
            view.print_message(e.what());
        }
        
        file.close();
    }
};

void Game::save_game(){
    logger.log("save_game called");
    json game_data;
        
    game_data["game_lvl"] = game_lvl;
    game_data["hard_lvl"] = hard_lvl;
    game_data["win_points"] = win_points;
    
    game_data["field"] = field.to_json();
    game_data["hand"] = hand.to_json();
        
    std::ofstream file(SAVE_GAME_FILE);
    if (file.is_open()) {
        file << game_data.dump(4);
        file.close();
        view.print_message("Game saved");
    } else {
        view.print_message("Error: Could not save game!");
    }
};

void Game::level_up(){
    game_lvl++;
    unsigned int lvl = game_lvl + hard_lvl - 1;
    win_points = 25 * ((7 * lvl - 3) / 4);

    unsigned int height, width;
    view.print_message("Please, enter height and width of field:");
    height = user_input.input_int();
    width = user_input.input_int();
    field = Field(height, width, game_lvl, hard_lvl);

    hand.delete_spells();
};

void Game::end_game(end_type type){
    logger.log("end_game called");
    char input;
    switch(type){
        case end_type::victory:
            view.print_message("Victory!");
            if(game_lvl < 3){
                level_up();
            }else{
                view.print_new_exit(control);
                while(true){
                    input = user_input.input_char();
                    if(input == control.yes) break;
                    if(input == control.exit) throw ExitException();
                }
                init_game();
            }
            break;
        case end_type::defeat:
            view.print_new_exit(control);
            while(true){
                input = user_input.input_char();
                if(input == control.yes) break;
                if(input == control.exit) throw ExitException();
            }
            init_game();
            break;
        case end_type::user_end:
            view.print_save_exit(control);
            while(true){
                input = user_input.input_char();
                if(input == control.save) break;
                if(input == control.exit) throw ExitException();
            }
            save_game();
            throw ExitException();
        case end_type::crash:
            throw ExitException();
    }
};

void Game::help(){
    std::ostringstream oss;
    view.print_message("\033c     _     _\n|_| |_ |  |_| .\n| | |_ |_ |   .\n\n");
    oss << "Your goal is to score " << win_points << " points.";
    view.print_message(oss.str());
    view.print_message("You will receive points for killing enemies and destroying enemy buildings.");
    view.print_message("To move enter coordinates of target cell.");
    view.print_message("Let's go!");
    user_input.input_enter();
};

void Game::Hero_turn(){
    logger.log("Hero_turn called");
    Hero* hero = static_cast<Hero*>(field.cells[field.hero_coord[1]][field.hero_coord[0]].occupied);
    char action;
    std::ostringstream oss;
    view.print_game_state(field, *hero, hand, win_points);
    while(true){
        view.print_turn_menu(control, hand.can_use_spell());
        action = user_input.input_char();
        if(action == control.exit || action == control.move || action == control.change_attack || 
                (hand.can_use_spell() && action == control.use_spell)){
            break;
        }
    }
    if(action == control.exit){
        end_game(end_type::user_end);
    }
    std::array<unsigned int, 2> coords;
    if(action == control.move || action == control.change_attack){
        if(action == control.move){
            coords = user_input.input_coords(field.height, field.width);
            field.move_character(field.hero_coord[0], field.hero_coord[1], coords[0], coords[1]);
            if(!(field.cells[field.hero_coord[1]][field.hero_coord[0]].occupied)) end_game(end_type::defeat);
            view.print_game_state(field, *hero, hand, win_points);
        }else if(action == control.change_attack){
            hero->change_attack();
        }
        oss << "You can attack in the range of " << hero->range() << " cells";
        view.print_message(oss.str());
        coords = user_input.input_coords_in_range(field.hero_coord[0], field.hero_coord[1], 
                field.height, field.width, hero->range());
        bool give_spell = field.damage_cell(coords[0], coords[1], hero->attack());
        if(give_spell){
            view.print_game_state(field, *hero, hand, win_points);
            hand.get_spell(game_lvl);
        }
    }else{
        char index;
        while(true){
            view.print_message("Choose spell:");
            index = user_input.input_char();
            if('1' <= index && index < '9'){
                break;
            }
        }
        Spell_stats stats = hand.use_spell((unsigned int)(index - '1'));
        bool give_spell;
        switch(stats.type){
            case spell_type::direct_damage:
                oss << "You can apply direct damage in the range of " << stats.range << " cells";
                view.print_message(oss.str());
                coords = user_input.input_coords_in_range(field.hero_coord[0], field.hero_coord[1], 
                        field.height, field.width, stats.range);
                give_spell = field.damage_cell(coords[0], coords[1], stats.damage);
                if(give_spell){
                    view.print_game_state(field, *hero, hand, win_points);
                    hand.get_spell(game_lvl);
                }
                break;
            case spell_type::area_damage:
                oss << "You can apply area damage in the range of " << stats.range << " cells\n";
                view.print_message(oss.str());
                coords = user_input.input_coords_in_range(field.hero_coord[0], field.hero_coord[1], 
                        field.height, field.width, stats.range);
                for(int i = 0; i < stats.area_side; i++){
                    for(int j = 0; j < stats.area_side; j++){
                        give_spell = field.damage_cell(coords[0] + stats.area_side/2 + i, coords[1] + stats.area_side/2 + j, stats.damage);
                        if(give_spell){
                            view.print_game_state(field, *hero, hand, win_points);
                            hand.get_spell(game_lvl);
                        }
                    }
                }
                break;
            case spell_type::trap:
                oss << "You can set trap in the range of 1 cell\n";
                view.print_message(oss.str());
                coords = user_input.input_coords_in_range(field.hero_coord[0], field.hero_coord[1], 
                        field.height, field.width, stats.range);
                field.set_trap(coords[0], coords[1], stats.damage);
                view.print_game_state(field, *hero, hand, win_points);
                break;
            case spell_type::call_ally:
                field.call_ally(field.hero_coord[0], field.hero_coord[1], game_lvl, hard_lvl);
                view.print_game_state(field, *hero, hand, win_points);
                break;
        }
    }
    if(hero->win(win_points)) end_game(end_type::victory);
};

void Game::Ally_turn(){
    logger.log("Ally_turn called");
    Hero* hero = static_cast<Hero*>(field.cells[field.hero_coord[1]][field.hero_coord[0]].occupied);
    std::ostringstream oss;
    int i = 0;
    while(i < field.character_coord.size()){
        unsigned int ally_x = field.character_coord[i][0];
        unsigned int ally_y = field.character_coord[i][1];
        if(field.cells[ally_y][ally_x].occupied->get_type() == character_type::ally){
            Ally* ally = static_cast<Ally*>(field.cells[ally_y][ally_x].occupied);
            unsigned int target_x = 1000;
            unsigned int target_y = 1000;
            unsigned int target_index;
            for(int j = 0; j < field.character_coord.size(); j++){
                unsigned int enemy_x = field.character_coord[j][0];
                unsigned int enemy_y = field.character_coord[j][1];
                if(field.cells[enemy_y][enemy_x].occupied->get_type() != character_type::ally && 
                        abs((int)target_x - (int)ally_x) + abs((int)target_y - (int)ally_y) >= abs((int)enemy_x - (int)ally_x) + abs((int)enemy_y - (int)ally_y)){
                    target_x = enemy_x;
                    target_y = enemy_y;
                    target_index = j;
                }
            }
            unsigned int count = field.character_coord.size();
            oss << "Ally from cell " << (char)(ally_x + 'a') << " " << ally_y + 1 << " is acting...\n";
            view.print_message(oss.str());
            user_input.input_enter();
            field.move_character(ally_x, ally_y, target_x, target_y);
            view.print_game_state(field, *hero, hand, win_points);

            if(field.character_coord.size() < count) continue;
            ally_x = field.character_coord[i][0];
            ally_y = field.character_coord[i][1];
            if(abs((int)target_x - (int)ally_x) + abs((int)target_y - (int)ally_y) > ally->range()){
                i++;
                continue;
            }
            bool give_spell = field.damage_cell(target_x, target_y, ally->attack());
            if(give_spell){
                hand.get_spell(game_lvl);
            }
            if(field.character_coord[target_index][0] == target_x && field.character_coord[target_index][1] == target_y){
                i++; // enemy living
            }else{
                user_input.input_enter();
                view.print_game_state(field, *hero, hand, win_points);
                if(i < target_index){
                    i++;
                }
            }
        }else{
            i++;        
        }
    }
};

void Game::Enemy_turn(){
    logger.log("Enemy_turn called");
    Hero* hero = static_cast<Hero*>(field.cells[field.hero_coord[1]][field.hero_coord[0]].occupied);
    std::ostringstream oss;
    int i = 0;
    while(i < field.character_coord.size()){
        unsigned int enemy_x = field.character_coord[i][0];
        unsigned int enemy_y = field.character_coord[i][1];
        if(field.cells[enemy_y][enemy_x].occupied->get_type() == character_type::enemy){
            Enemy* enemy = static_cast<Enemy*>(field.cells[enemy_y][enemy_x].occupied);
            unsigned int target_x = field.hero_coord[0];
            unsigned int target_y = field.hero_coord[1];
            unsigned int target_index = i;
            for(int j = 0; j < field.character_coord.size(); j++){
                unsigned int ally_x = field.character_coord[j][0];
                unsigned int ally_y = field.character_coord[j][1];
                if(field.cells[ally_y][ally_x].occupied->get_type() == character_type::ally && 
                        abs((int)target_x - (int)ally_x) + abs((int)target_y - (int)ally_y) >= 
                        abs((int)enemy_x - (int)ally_x) + abs((int)enemy_y - (int)ally_y)){
                    target_x = enemy_x;
                    target_y = enemy_y;
                    target_index = j;
                }
            }
            unsigned int count = field.character_coord.size();
            oss << "Enemy from cell " << (char)(enemy_x + 'a') << " " << enemy_y + 1 << " is acting...\n";
            view.print_message(oss.str());
            user_input.input_enter();
            field.move_character(enemy_x, enemy_y, target_x, target_y);
            view.print_game_state(field, *hero, hand, win_points);

            if(field.character_coord.size() < count) continue;
            enemy_x = field.character_coord[i][0];
            enemy_y = field.character_coord[i][1];
            if(abs((int)target_x - (int)enemy_x) + abs((int)target_y - (int)enemy_y) > enemy->range()){
                i++;
                continue;
            }
            field.damage_cell(target_x, target_y, enemy->attack());
            if(!(field.cells[field.hero_coord[1]][field.hero_coord[0]].occupied)) end_game(end_type::defeat);
            if(field.character_coord[target_index][0] == target_x && field.character_coord[target_index][1] == target_y){
                i++; // ally living
            }else{
                user_input.input_enter();
                view.print_game_state(field, *hero, hand, win_points);
                if(i < target_index){
                    i++;
                }
            }
        }else{
            i++;
        }
    }
};

void Game::Base_turn(){
    logger.log("Base_turn called");
    Hero* hero = static_cast<Hero*>(field.cells[field.hero_coord[1]][field.hero_coord[0]].occupied);
    std::ostringstream oss;
    int i = 0;
    while(i < field.character_coord.size()){
        unsigned int base_x = field.character_coord[i][0];
        unsigned int base_y = field.character_coord[i][1];
        if(field.cells[base_y][base_x].occupied->get_type() == character_type::base){
            oss << "Base from cell " << (char)(base_x + 'a') << " " << base_y + 1 << " is acting...\n";
            view.print_message(oss.str());
            field.make_enemy(base_x, base_y, game_lvl, hard_lvl);
            user_input.input_enter();
            view.print_game_state(field, *hero, hand, win_points);
        }
        i++;
    }
};

void Game::Tower_turn(){
    logger.log("Tower_turn called");
    Hero* hero = static_cast<Hero*>(field.cells[field.hero_coord[1]][field.hero_coord[0]].occupied);
    std::ostringstream oss;
    int i = 0;
    while(i < field.character_coord.size()){
        unsigned int tower_x = field.character_coord[i][0];
        unsigned int tower_y = field.character_coord[i][1];
        if(field.cells[tower_y][tower_x].occupied->get_type() == character_type::tower){
            Tower* tower = static_cast<Tower*>(field.cells[tower_y][tower_x].occupied);
            unsigned int hero_x = field.hero_coord[0];
            unsigned int hero_y = field.hero_coord[1];
            oss << "Tower from cell " << (char)(tower_x + 'a') << " " << tower_y + 1 << " is acting...\n";
            view.print_message(oss.str());
            user_input.input_enter();
            if(abs((int)hero_x - (int)tower_x) + abs((int)hero_y - (int)tower_y) <= tower->range()){
                field.damage_cell(hero_x, hero_y, tower->attack());
                if(!(field.cells[hero_y][hero_x].occupied)) end_game(end_type::defeat);
            }
            int j = 0; 
            while(j < field.character_coord.size()){
                unsigned int ally_x = field.character_coord[j][0];
                unsigned int ally_y = field.character_coord[j][1];
                if(field.cells[ally_y][ally_x].occupied->get_type() == character_type::ally && 
                        abs((int)hero_x - (int)tower_x) + abs((int)hero_y - (int)tower_y) <= tower->range()){
                    unsigned int count = field.character_coord.size();
                    field.damage_cell(ally_x, ally_y, tower->attack());
                    if(field.character_coord.size() < count){
                        user_input.input_enter();
                        view.print_game_state(field, *hero, hand, win_points);
                        if(i > j) i--;
                    }
                }else{
                    j++;
                }
            }
        }
        i++;
    }
};