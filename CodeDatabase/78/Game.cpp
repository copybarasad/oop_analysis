#include "Game.h"
#include "SaveLoadExceptions.h"

#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <limits>
#include <unistd.h>

void Game::show_main_menu() {
    system("cls");
    std::cout << "=== MAIN MENU ===" << std::endl;
    std::cout << "1. New game" << std::endl;
    std::cout << "2. Load game" << std::endl;
    std::cout << "3. Exit" << std::endl;
}

void Game::start(){
    int attempts = 20;
    show_main_menu();
    int choice = get_validated_input("Select action: ", 1, 3);
    invalid_input();

    switch(choice){
        case 1:
            current_level_ = 1;
            initialize_current_level();
            run();
            break;
        case 2:
            show_save_files();
            run();
            break;
        case 3:
            std::cout << "Goodbye" << std::endl;
            break;
    }
}

void Game::initialize_current_level(){
    int size = 10 + (current_level_ - 1) * 3;
    if(size > 25) size = 25;
    std::unique_ptr<GameField> new_field = std::make_unique<GameField>(size, size);

    int middle = size / 2;

    if(current_level_ != 1 && field_ && field_->get_player()){
        new_field->place_entity(std::move(field_->extract_player()), middle, middle);
    }else{
        new_field->place_entity(std::make_unique<Player>(middle, middle), middle, middle);
    }
    auto player = new_field->get_player();

    player->set_position(middle, middle);
    int size_spell_hand = player->get_spell_hand().get_size();
    if(size_spell_hand > 1){
        for(int i = 0; i < size_spell_hand / 2; i++){
            int index = rand() % size_spell_hand;
            if(player->get_spell_hand().remove_spell(index)) size_spell_hand--;
        }
    }

    if(current_level_ != 1){
        player->heal_full();
    }

    new_field->spawn_random_towers((current_level_ + 1) / 3, 100 + (current_level_ - 1) * 20, 2, 10,\
                                        50 + 10 * current_level_, 0.2*current_level_);
    new_field->spawn_random_enemies(current_level_, 50 + (current_level_ - 1) * 5);
    new_field->spawn_random_cell_type(current_level_ * 5, CellType::WALL);
    new_field->spawn_random_cell_type(current_level_ * 2, CellType::SLOW_ZONE);

    field_ = std::move(new_field);
}

void Game::run(){
    game_running_ = true;
    
    while(game_running_ && field_->get_player()->is_alive()){
        system("cls");
        std::cout << "=== Level " << current_level_ << " ===" << std::endl;
        field_->draw_field();
        
        process_player_turn();
        field_->update();

        if(!game_running_) break;

        if(check_win_condition()){
            level_complete();
            continue;
        }

        if(check_lose_condition()){
            game_over();
        }
    }
}

void Game::process_player_turn(){
    bool action_performed = false;
    std::string command;

    while(!action_performed && game_running_){
        std::cout << "Command: ";
        std::cin >> command;
        invalid_input();

        if (command == "help") {
            std::cout << "\n=== AVAILABLE COMMANDS ===" << std::endl;
            std::cout << "MOVEMENT:" << std::endl;
            std::cout << "  w, a, s, d  OR  move [w/a/s/d]" << std::endl;
            std::cout << "ATTACK:" << std::endl;
            std::cout << "  e [dx] [dy]  OR  attack [dx] [dy]" << std::endl;
            std::cout << "WEAPONS:" << std::endl;
            std::cout << "  r [1-4]      OR  weapon [sword/bow/staff/fists]" << std::endl;
            std::cout << "SPELLS:" << std::endl;
            std::cout << "  c            OR  spell - cast spell" << std::endl;
            std::cout << "  b            OR  shop - spell shop" << std::endl;
            std::cout << "INFO:" << std::endl;
            std::cout << "  stat         - detailed statistics" << std::endl;
            std::cout << "  enemy/list   - enemies and towers stats" << std::endl;
            std::cout << "  help         - this help message" << std::endl;
            std::cout << "  q            OR  quit - exit" << std::endl;
            continue;
        }
        
        if (command == "move" || command == "w" || command == "a" || command == "s" || command == "d") {
            char direction;
            if (command.length() == 1) {
                direction = command[0];
            } else {
                std::cin >> direction;
                invalid_input();
            }
            
            switch (direction) {
                case 'w': action_performed = field_->get_player()->move(*field_, 0, -1); break;
                case 'a': action_performed = field_->get_player()->move(*field_, -1, 0); break;
                case 's': action_performed = field_->get_player()->move(*field_, 0, 1); break;
                case 'd': action_performed = field_->get_player()->move(*field_, 1, 0); break;
                default: 
                    std::cout << "Invalid direction! Use w/a/s/d" << std::endl;
                    continue;
            }
        }else if (command == "attack" || command == "e") {
            int dx, dy;
            std::cin >> dx >> dy;
            invalid_input();
            action_performed = field_->get_player()->attack(*field_, dx, dy);
        }else if (command == "weapon" || command == "r") {
            std::string weapon_input;
            std::cin >> weapon_input;
            invalid_input();
            
            WeaponType new_weapon;
            
            if (weapon_input.length() == 1) {
                int weapon_choice = weapon_input[0] - '0';
                
                switch(weapon_choice) {
                    case 1: new_weapon = WeaponType::SWORD; break;
                    case 2: new_weapon = WeaponType::BOW; break;
                    case 3: new_weapon = WeaponType::STAFF; break;
                    case 4: new_weapon = WeaponType::FISTS; break;
                    default:
                        std::cout << "Invalid weapon number! Use 1-4" << std::endl;
                        continue;
                }
            } else {
                if (weapon_input == "sword") new_weapon = WeaponType::SWORD;
                else if (weapon_input == "bow") new_weapon = WeaponType::BOW;
                else if (weapon_input == "staff") new_weapon = WeaponType::STAFF;
                else if (weapon_input == "fists") new_weapon = WeaponType::FISTS;
                else {
                    std::cout << "Invalid weapon! Available: sword, bow, staff, fists" << std::endl;
                    continue;
                }
            }
            field_->get_player()->switch_weapon(new_weapon);
            std::cout << "Weapon switched to: " << weapon_input << std::endl;
            action_performed = true;
        }else if (command == "spell" || command == "c") {
            std::cout << "\n=== YOUR SPELLS ===" << std::endl;
            auto& spell_hand = field_->get_player()->get_spell_hand();
            for (int i = 0; i < spell_hand.get_size(); ++i) {
                if (spell_hand.get_spell(i)) {
                    auto* spell = spell_hand.get_spell(i);
                    std::cout << i + 1 << ". " << spell->get_name() << " LVL " << spell->get_level();
                    std::cout << " (Mana: " << spell->get_mana_cost() << ")";
                    std::cout << " - " << spell->get_description() << std::endl;
                } else {
                    std::cout << i + 1 << ". Empty slot" << std::endl;
                }
            }
            
            std::cout << "Select spell (1-" << spell_hand.get_size() << ") or 0 to cancel: ";
            int spell_index;
            std::cin >> spell_index;
            invalid_input();
            
            if (spell_index == 0) continue;
            
            if (spell_index < 1 || spell_index > spell_hand.get_size()) {
                std::cout << "Invalid spell number!" << std::endl;
                continue;
            }
            
            if (!spell_hand.get_spell(spell_index - 1)) {
                std::cout << "This spell slot is empty!" << std::endl;
                continue;
            }
            
            int target_x = -1, target_y = -1;
            auto* spell = spell_hand.get_spell(spell_index - 1);
            
            if (spell->needs_target()) {
                std::cout << "Enter target coordinates (x y): ";
                std::cin >> target_x >> target_y;
                invalid_input();
                
                if (!field_->is_coordinates_valid(target_x, target_y)) {
                    std::cout << "Invalid coordinates!" << std::endl;
                    continue;
                }
            }
            
            if (field_->get_player()->cast_spell(*field_, spell_index - 1, target_x, target_y)) {
                std::cout << "Spell cast successfully!" << std::endl;
                action_performed = true;
            } else {
                std::cout << "Failed to cast spell!" << std::endl;
                continue;
            }
        }else if(command == "save"){
            if(!save_game()){
                std::cout << "Failed to save game!" << std::endl;
            }
        }else if(command == "load"){
            show_save_files();
            return;
        }else if (command == "shop" || command == "b") {
            field_->get_player()->spell_shop();
        }else if (command == "stat" || command == "stats") {
            field_->get_player()->show_stats();
            std::cout << "\n=== SPELLS ===" << std::endl;
            auto& spell_hand = field_->get_player()->get_spell_hand();
            for (int i = 0; i < spell_hand.get_size(); ++i) {
                if (spell_hand.get_spell(i)) {
                    std::cout << "Slot " << i + 1 << ": " << spell_hand.get_spell(i)->get_name();
                    std::cout << " (Mana: " << spell_hand.get_spell(i)->get_mana_cost() << ")" << std::endl;
                }
            }
            wait_moves();
        }else if (command == "enemy" || command == "list") {
            field_->show_enemy_stats();
            wait_moves();
        }else if (command == "quit" || command == "q") {
            std::cout << "Exiting game..." << std::endl;
            game_running_ = false;
            return;
        }else {
            std::cout << "Unknown command '" << command << "'. Type 'help' for commands list." << std::endl;
            continue;
        }
        
        if (action_performed && field_->get_player()->is_on_slow_cell(*field_)) {
            std::cout << "You are on a slow cell, you skip one move!" << std::endl;
            field_->update();
            wait_moves();
        }
    }
}

void Game::level_complete() {
    std::cout << "=== LEVEL " << current_level_ << " COMPLETED! ===" << std::endl;
    field_->get_player()->show_level_up_menu();
    
    std::cout << "1. Next Level" << std::endl;
    std::cout << "2. Save and Exit" << std::endl;
    
    int choice = get_validated_input("Select action: ", 1, 2);
    
    if (choice == 1) {
        current_level_++;
        initialize_current_level();
    } else {
        save_game();
        game_running_ = false;
    }
}

void Game::game_over() {
    std::cout << "=== GAME OVER ===" << std::endl;
    std::cout << "Your result:" << std::endl;
    if (field_) {
        field_->get_player()->show_stats();
    }
    
    std::cout << "\n1. New Game" << std::endl;
    std::cout << "2. Exit" << std::endl;
    
    int choice = get_validated_input("Select action: ", 1, 2);
    
    if (choice == 1) {
        current_level_ = 1;
        initialize_current_level();
    } else {
        game_running_ = false;
    }
}

std::string Game::serialize() const {
    std::string data;

    data += "CURRENT_LEVEL " + std::to_string(current_level_) + "\n\n";
    data += field_->serialize();
    data += field_->get_player()->serialize();

    return data;
}

bool Game::save_game() {
    try {
        std::string save_dir = "saves";
        if (!std::filesystem::exists(save_dir)) {
            std::filesystem::create_directory(save_dir);
        }

        std::string full_path;
        std::string filename;

        filename = "Level_" + std::to_string(current_level_) + "_time_" + std::to_string(std::time(nullptr)) + ".txt";
        full_path = save_dir + "/" + filename;

        std::ofstream file(full_path);
        if (!file) {
            throw SaveGameException("Cannot create file: " + full_path);
        }

        file << serialize();
        if (file.fail()) {
            throw SaveGameException("Write error in file: " + full_path);
        }
        
        std::cout << "Game saved successfully: " << full_path << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Save game error: " << e.what() << std::endl;
        return false;
    }
}

void Game::show_save_files() {
    std::cout << "=== SAVE FILES ===" << std::endl;
    
    std::vector<std::string> save_files;
    std::string save_dir = "saves";
    
    if (!std::filesystem::exists(save_dir)) {
        std::filesystem::create_directory(save_dir);
    }
    
    try {
        int index = 1;
        for (const auto& entry : std::filesystem::directory_iterator(save_dir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                std::cout << index++ << ". " << entry.path().filename().string() << std::endl;
                save_files.push_back(entry.path().string());
            }
        }
        
        if (save_files.empty()) {
            std::cout << "No save files found." << std::endl;
            wait_moves();
            return;
        }
        
        std::cout << "0. Back to main menu" << std::endl;
        int choice = get_validated_input("Select save file: ", 0, save_files.size());
        
        if (choice > 0) {
            if (load_game(save_files[choice - 1])) {
                wait_moves();
            } else {
                std::cout << "Failed to load game!" << std::endl;
                wait_moves();
            }
        }
        
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error accessing save directory: " << e.what() << std::endl;
        wait_moves();
    }
}

bool Game::load_game(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file) {
            throw LoadGameException("Cannot open file: " + filename);
        }

        std::string line;
        bool has_player = false;
        bool has_field = false;

        while (std::getline(file, line)) {
            if (line.empty()) continue;

            std::istringstream stream(line);
            std::string key;
            stream >> key;

            if (key == "CURRENT_LEVEL") {
                if (!(stream >> current_level_)) {
                    throw LoadGameException("Invalid CURRENT_LEVEL format in: " + filename);
                }
                if (current_level_ < 1) {
                    throw LoadGameException("Invalid level number: " + std::to_string(current_level_));
                }
            }
            else if (key == "PLAYER") { deserialize_player(file); has_player = true;}
            else if (key == "FIELD") { deserialize_field(file); has_field = true; }
            else if (key == "ENEMY") { deserialize_enemy(file); }
            else if (key == "TOWER") { deserialize_tower(file); }
            else if (key == "ALLY") { deserialize_ally(file); }
            else if (!key.empty()) {
                throw LoadGameException("Unknown section: '" + key + "' in: " + filename);
            }
        }

        if (!has_player) {
            throw LoadGameException("Missing PLAYER section in: " + filename);
        }
        if (!has_field) {
            throw LoadGameException("Missing FIELD section in: " + filename);
        }

        if (!field_ || !field_->get_player()) {
            throw LoadGameException("Game state corrupted after load: " + filename);
        }

        game_running_ = true;
        std::cout << "Game loaded successfully from: " << filename << std::endl;
        return true;

    } catch (const LoadGameException& e) {
        std::cerr << e.what() << std::endl;
        field_.reset();
        game_running_ = false;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error during load: " << e.what() << std::endl;
        field_.reset();
        game_running_ = false;
        return false;
    }
}

void Game::deserialize_field(std::ifstream& file){
    std::string line;

    std::getline(file, line);
    std::istringstream size_stream(line);
    std::string key;
    int width, height;
    size_stream >> key >> width >> height;
    field_ = std::make_unique<GameField>(width, height);

    for(int y = 0; y < height && std::getline(file, line); y++){
        for(int x = 0; x < width; x++){
            char symbol = line[x];
            switch(symbol){
                case '#': field_->get_cell(x, y).set_type(CellType::WALL); break;
                case '~': field_->get_cell(x, y).set_type(CellType::SLOW_ZONE); break;
                case 't': field_->get_cell(x, y).set_type(CellType::TRAP); break;
            }
        }
    }
}

void Game::deserialize_enemy(std::ifstream& file){
    std::string line;
    int health = 0, max_health = 0, x = 0, y = 0;

    while(std::getline(file, line)){
        if(line.empty()) break;

        std::istringstream stream(line);
        std::string key;

        stream >> key;
        if (key == "HEALTH") stream >> health;
        else if (key == "MAX_HEALTH") stream >> max_health;
        else if (key == "POSITION")stream >> x >> y;
    }

    if(field_ && field_->is_coordinates_valid(x, y)){
        auto enemy = std::make_unique<Enemy>(max_health, x, y);
        enemy->set_health(health);
        field_->place_entity(std::move(enemy), x, y);
    }
}

void Game::deserialize_tower(std::ifstream& file){
    std::string line;
    int health = 0, max_health = 0, x = 0, y = 0;
    int range_spawn, cooldown_spawn, max_cooldown_spawn;
    int cooldown_attack, max_cooldown_attack;
    int health_spawned_enemy;
    float damage_multiplier;

    while(std::getline(file, line)){
        if(line.empty()) break;

        std::istringstream stream(line);
        std::string key;

        stream >> key;
        if (key == "HEALTH") stream >> health;
        else if (key == "MAX_HEALTH") stream >> max_health;
        else if (key == "POSITION") stream >> x >> y;

        else if (key == "RANGE_SPAWN") stream >> range_spawn; 
        else if (key == "COOLDOWN_SPAWN") stream >> cooldown_spawn >> max_cooldown_spawn;
        else if (key == "COOLDOWN_ATTACK") stream >> cooldown_attack >> max_cooldown_attack;
        else if (key == "HEALTH_SE") stream >> health_spawned_enemy;
        else if (key == "MULTIPLIER") stream >> damage_multiplier;
    }

    if(field_ && field_->is_coordinates_valid(x, y)){
        auto tower = std::make_unique<Tower>(max_health, x, y, range_spawn, max_cooldown_spawn, max_cooldown_attack, health_spawned_enemy, damage_multiplier);
        tower->set_health(health);
        tower->set_cooldown_spawn(cooldown_spawn);
        tower->set_cooldown_attack(cooldown_attack);

        field_->place_entity(std::move(tower), x, y);
    }
}

void Game::deserialize_ally(std::ifstream& file){
    std::string line;
    int health = 0, max_health = 0, x = 0, y = 0;

    while(std::getline(file, line)){
        if(line.empty()) break;

        std::istringstream stream(line);
        std::string key;

        stream >> key;
        if (key == "HEALTH") stream >> health;
        else if (key == "MAX_HEALTH") stream >> max_health;
        else if (key == "POSITION") stream >> x >> y;
    }

    if(field_ && field_->is_coordinates_valid(x, y)){
        auto ally = std::make_unique<Ally>(max_health, x, y);
        ally->set_health(health);
        field_->place_entity(std::move(ally), x, y);
    }
}

void Game::deserialize_player(std::ifstream& file) {
    std::string line;
    int health = 0, max_health = 0, x = 0, y = 0;
    int level = 0, score = 0, score_for_next = 0, gold = 0;
    int mana = 0, max_mana = 0, hand_size;
    float damage_multiplier;
    WeaponType weapon = WeaponType::SWORD;
    std::vector<std::string> spell_names;
    std::vector<int> spell_levels;

    while (std::getline(file, line)) {
        if (line.empty()) break;

        std::istringstream stream(line);
        std::string key;
        std::string weapon_str;
        
        stream >> key;
        if (key == "HEALTH") stream >> health;
        else if (key == "MAX_HEALTH") stream >> max_health;
        else if (key == "WEAPON") {
            stream >> weapon_str;
            if (weapon_str == "Sword") weapon = WeaponType::SWORD;
            else if (weapon_str == "Bow") weapon = WeaponType::BOW;
            else if (weapon_str == "Staff") weapon = WeaponType::STAFF;
            else weapon = WeaponType::FISTS;
        }
        else if (key == "POSITION") stream >> x >> y;
        else if (key == "LEVEL") stream >> level;
        else if (key == "SCORE") stream >> score;
        else if (key == "SCORE_FOR_NEXT_LEVEL") stream >> score_for_next;
        else if (key == "GOLD") stream >> gold;
        else if (key == "MANA") stream >> mana;
        else if (key == "MAX_MANA") stream >> max_mana;
        else if (key == "DAMAGE_MULTIPLIER") stream >> damage_multiplier;
        else if (key == "SPELL_HAND") {
            stream >> hand_size;

            std::string spell_name;
            int spell_level;
        while (stream >> spell_name >> spell_level) {
            if (!spell_name.empty()) {
                spell_names.push_back(spell_name);
                spell_levels.push_back(spell_level);
            }
        }
        }
    }

    auto player = std::make_unique<Player>(x, y, max_health, max_mana, hand_size);
    for (int i = player->get_spell_hand().get_size() - 1; i >= 0; --i) {
        player->get_spell_hand().remove_spell(i);
    }

    for (int i = 0; i < spell_names.size(); i++) {
        ISpell* spell = create_spell_from_name(spell_names[i], spell_levels[i]);
        if (spell) {
            player->get_spell_hand().add_spell(spell);
        }
    }

    player->set_health(health);
    player->set_level(level);
    player->set_score(score);
    player->set_score_for_next_level(score_for_next);
    player->set_damage_multiplier(damage_multiplier);
    player->switch_weapon(weapon);
    player->add_gold(gold - player->get_gold());
    player->add_mana(mana - player->get_mana());

    field_->place_entity(std::move(player), x, y);
}

ISpell* Game::create_spell_from_name(const std::string& name, int level) {
    if (name == "DirectDamage") return new DirectDamageSpell(level);
    else if (name == "AreaDamage") return new AreaDamageSpell(level);
    else if (name == "Trap") return new TrapSpell(level);
    else if (name == "SummonAlly") return new SummonSpell(level);
    else if (name == "Enhancement") return new EnhancementSpell(level);
    else return nullptr;
}

bool Game::check_win_condition(){
    return field_->get_enemies().empty() && field_->get_towers().empty();
}

bool Game::check_lose_condition(){
    return !field_->get_player()->is_alive();
}

void invalid_input(){
    if (!std::cin) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input!" << std::endl;
    }
}

void wait_moves(){
    std::cout << "Press Enter to continue";
    std::cin.ignore();
    std::cin.get();
}


int Game::get_validated_input(const std::string& prompt, int min, int max){
    int choice;
    int attempts = 10;
    while(attempts-- > 0){
        std::cout << prompt;
        std::cin >> choice;
        if(std::cin.fail() || choice < min || choice > max){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Enter a number from " << min << " to " << max << std::endl;
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
    }

    return -1;
}