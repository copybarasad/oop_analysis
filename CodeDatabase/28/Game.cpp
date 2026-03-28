#include "Game.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdexcept>
#include <algorithm>
#include <ctime>
#include <limits>

Game::Game() : hand(5), current_state(GameState::MAIN_MENU) {
    srand(time(nullptr));
}

void Game::Run() {
    while (current_state != GameState::EXIT) {
        switch (current_state)
        {
        case GameState::MAIN_MENU:
            show_main_menu();
            break;
        case GameState::PLAYING:
            process_game_loop();
            break;
        case GameState::LEVEL_COMPLETE:
            level_complete();
            break;
        case GameState::GAME_OVER:
            game_over();
            break;
        case GameState::EXIT:
            break;
        }
    }
}

// --------- МЕНЮ ---------

void Game::show_main_menu() {
    int choice = 0;
    bool is_running = true;

    while (is_running) {
        std::cout << "- HERO GAME -\n"
                  << "1. New Game\n"
                  << "2. Load Game\n"
                  << "3. Exit\n"
                  << "Select option: ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (choice == 1) {
            start_new_game();
            is_running = false;
        } else if (choice == 2) {
            try {
                load_game();
                if(!(current_state == GameState::EXIT)){
                    current_state = GameState::PLAYING;
                    is_running = false;
                }
            } catch (const std::exception& ex) {
                std::cout << ex.what() << "\n";
            }
        } else if (choice == 3) {
            current_state = GameState::EXIT;
            is_running = false;
        }
    }
}

void Game::start_new_game() {
    reset_game_state();
    current_level = 1;
    player = std::make_unique<Player>(100, 0, 0, 5);
    level_init();
    current_state = GameState::PLAYING;
}

void Game::game_over() {
    std::cout << "\nGAME OVER\n";
    std::cout << "1. Restart from Level 1\n";
    std::cout << "2. Exit\n";
    std::cout << "Choice: ";
    int choice = 0;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        choice = 2;
    }

    if (choice == 1) {
        start_new_game();
    } else {
        current_state = GameState::EXIT;
    }
}


// --------- УРОВНИ / ИНИЦИАЛИЗАЦИЯ / ПРОКАЧКА ---------

void Game::level_complete(){
    std::cout << "\nLEVEL " << current_level << " COMPLETE\n";
    reset_game_state();
    if (current_level >= MAX_LEVELS) {
        std::cout << "You completed all levels. Congratulations!\n";
        std::cout << "1. Start new game\n2. Exit\nChoice: ";
        int choice = 0;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            choice = 2;
        }
        if (choice == 1) {
            start_new_game();
        } else {
            current_state = GameState::EXIT;
        }
        return;
    }
    level_upgrade();
    current_level++;
    player->get_hand()->remove_half_spells();
    level_init();
    current_state = GameState::PLAYING;
}

void Game::level_init() {
    int level = current_level;
    int size = DEFAULT_FIELD_SIZE + (level - 1) * 2;
    field = std::make_unique<Gamefield>(size, size);

    player->heal(player->get_max_hp());
    Cell* player_cell = field->get_random_cell();
    if (player_cell) {
        player->go_to(*player_cell);
    }

    int enemy_count = 1 + level;

    for (int i = 0; i < enemy_count; ++i) {
        Cell* enemy_cell = field->get_random_cell();
        if (enemy_cell) {
            auto e = std::make_unique<Enemy>(50, MIN_ENEMY_DMG + level * 5, enemy_cell->get_x(), enemy_cell->get_y());
            e->go_to(*enemy_cell);
            enemies.push_back(std::move(e));
        } else {
            std::cerr << "Enemy spawn cell not found!" << std::endl;
            continue;
        }
    }

    // После первого уровня размещается башня
    if (level >= 2) {
        int radius = 3 + level;
        int damage = 8 + level * 2;
        Cell* enemy_tower_cell = field->get_random_cell();
        enemy_tower = std::make_unique<EnemyTower>(enemy_tower_cell, 3);
        std::cout << "Enemy Tower at: (" << enemy_tower->get_x() << "," << enemy_tower->get_y() << ")\n";
    }
}

void Game::reset_game_state() {
    enemies.clear();
    enemy_tower.reset();
    field.reset();
}

void Game::level_upgrade() {
    std::cout << "\n====LEVEL UPGRADE!====\n";
    std::cout << "Choose your upgrade:\n";
    std::cout << "1. Increase Max HP (+20)\n";
    std::cout << "2. Upgrade a Spell \n";
    std::cout << "Select option: ";

    int choice = 0;
    bool valid_choice = false;

    while (!valid_choice) {
        if (std::cin >> choice) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (choice == 1 || choice == 2){
                valid_choice = true;
            } else {
                std::cout << "Invalid choice. Try 1 or 2: ";
            }
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid choice. Try 1 or 2: ";
        }
    }

    if (choice == 1) {
        int new_max = player->get_max_hp() + 20;
        player->set_max_hp(new_max);
        player->heal(20);
        std::cout << "Max HP increased to " << new_max << "!\n";
    }
    else if (choice == 2) {
        Hand* hand = player->get_hand();
        const auto& spells = hand->get_spells();
        int idx = -1;
        bool valid_input = false;
        std::cout << "Available spells:\n";
        for (size_t i = 0; i < spells.size(); ++i) {
            std::cout << "|" << i << "| " << spells[i]->name() << " | R = " << spells[i]->radius() << " | Dmg = " << spells[i]->get_power() <<"\n";
        }
        while (!valid_input) {
            std::cout << "Select Spell by number: ";
            if (std::cin >> idx){
                if (idx >= 0 && static_cast<size_t>(idx) < spells.size()){
                    valid_input = true;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } else { std::cout << "Invalid index, try again. Enter command: "; }
            } else {
                std::cout << "Not a number, try again.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }

        std::cout << "You selected: " << spells[idx]->name() << "\n";
        spells[idx]->upgrade();
        std::cout << spells[idx]->name() << " upgraded! New damage: " << spells[idx]->get_power() << "\n";
    }

    std::cout << "Press Enter to continue.";
    std::cin.get();
}

// --------- ОСНОВНОЙ ЦИКЛ ---------

void Game::process_game_loop(){
    char input;
    while (current_state == GameState::PLAYING) {
        std::cout << "\nLEVEL " << current_level << "\n";

        for (auto& e : enemies)
            std::cout << "Enemy at (" << e->x_pos << "," << e->y_pos << ")  " << e->hp << "hp  " << e->get_damage() << "dmg\n";

        field->print();

        std::cout << "Player HP: " << player->hp << " | Damage: " << player->get_damage() <<  " | Score: " << player->get_score() <<"\n";
        player->start_turn();


        std::cout << "Q - Main Menu, G - Save game, L - Load game\n";
        std::cout << "WASD - move, C - Change weapon, F - Cast, B - Buy spell\nEnter command: ";

        Hand* hand = player->get_hand();
        const auto& spells = hand->get_spells();
        int idx = -1;
        process_inputs(input, spells, &idx);

        if (input == 'B' || input == 'b') {
            if (player->get_hand() && player->get_score() >= 10) {
                if (hand->add_random_spell()) {
                    player->edit_score(-10);
                    std::cout << "Purchased random spell!\n";
                } else { std::cout << "Hand is full. Cannot purchase.\n"; }
            } else { std::cout << "Not enough score (10) or no hand available.\n"; }

        } else if (input == 'C' || input == 'c') {
            player->switch_weapon();
            std::cout << "Weapon switched! New damage: " << player->get_damage() << "\n";

        } else if (input == 'F' || input == 'f') {
            std::cout << "You selected: " << spells[idx]->name() << "\n";
            std::cout << "Enter target coordinates (x y): ";
            int tx, ty;
            if (std::cin >> tx >> ty) {
                if (spells[idx]->cast(*player, *field, tx, ty)) {
                    std::cout << "Spell cast successfully.\n";
                } else {
                    std::cout << "Failed to cast spell.\n";
                }
            }
        } else if (input == 'Q' || input == 'q') {
            std::cout << "\n";
            current_state = GameState::MAIN_MENU;
        }

        int nx = player->x_pos;
        int ny = player->y_pos;
        switch (input) {
            case 'W': case 'w': ny--; break;
            case 'S': case 's': ny++; break;
            case 'A': case 'a': nx--; break;
            case 'D': case 'd': nx++; break;

            default: continue;
        }

        Cell* next_cell = field->get_cell(nx, ny);
        if (next_cell != nullptr){
            player->go_to(*next_cell);
        } else {
            continue;
        }

        remove_dead_enemies();
        for (auto& enemy : enemies) {
            enemy->move_towards_player(*player, *field);
        }

        if (enemy_tower){
            enemy_tower->try_attack(*player, *field);
        }

        check_game_over();
    }
}

void Game::process_inputs(char& input, const std::vector<std::unique_ptr<Spell>>& spells, int* idx) {
    bool valid_input = false;
    while (!valid_input){
        std::cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (input == 'B' || input == 'b' || input == 'C' || input == 'c' || input == 'W' || input == 'w'
            || input == 'A' || input == 'a' || input == 'S' || input == 's' || input == 'D' || input == 'd'
            || input == 'Q' || input == 'q') {
            valid_input = true;
        } else if (input == 'F' || input == 'f'){
            std::cout << "Available spells:\n";
            for (size_t i = 0; i < spells.size(); ++i) {
                std::cout << "|" << i << "| " << spells[i]->name() << " | R = " << spells[i]->radius() << " | Dmg = " << spells[i]->get_power() <<"\n";
            }
            while (!valid_input) {
                std::cout << "Select Spell by number: ";

                if (std::cin >> *idx){
                    if (*idx >= 0 && static_cast<size_t>(*idx) < spells.size()){
                        valid_input = true;
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    } else { std::cout << "Invalid index, try again. Enter command: "; }
                } else {
                    std::cout << "Not a number, try again.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
        } else if (input == 'G' || input == 'g') {
            try {
                save_game();
                std::cout << "Game saved.\n";
                valid_input = true;
            } catch (const std::exception& ex) {
                std::cout << ex.what() << "\n";
            }
            break;
        } else if (input == 'L' || input == 'l') {
            load_game();
            current_state = GameState::PLAYING;
            valid_input = true;
        } else {
            std::cout << "Incorrect input, try again. Enter command: ";
        }
    }
}


void Game::remove_dead_enemies() {
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::unique_ptr<Enemy>& enemy) {
                return enemy->hp <= 0;
            }
        ),
        enemies.end()
    );
}


void Game::check_game_over() {
    if (player->hp <= 0) {
        std::cout << "Game Over! The player has died\n";
        current_state = GameState::GAME_OVER;
    }

    if (enemies.empty()) {
        std::cout << "WIN! All enemies defeated\n";
        current_state = GameState::LEVEL_COMPLETE;
    }
}

// --------- SAVE / LOAD ---------
unsigned long long Game::calculate_hash(const std::string& data) {
    unsigned long long hash = 5381;
    for (char c : data) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

void Game::load_game() {
    try {
        load_gamefile();
        current_state = GameState::PLAYING;
        std::cout << "Game loaded successfully!\n";
    } catch (const std::exception& ex) {
        std::cout << "Load Error: " << ex.what() << "\n";
        current_state = GameState::EXIT;
    }
}

void Game::save_game() {
    if (!field || !player) {
        throw std::runtime_error("Nothing to save: game not started.");
    }

    std::stringstream ss;

    ss << current_level << "\n";
    ss << player->hp << " "
        << player->get_max_hp() << " "
        << player->get_score() << " "
        << player->get_x() << " "
        << player->get_y() << "\n";


    ss << field->get_width() << " " << field->get_length() << "\n";


    // 1 - проходима, 0 - стена
    for (int y = 0; y < field->get_width(); ++y) {
        for (int x = 0; x < field->get_length(); ++x) {
            Cell* cell = field->get_cell(x, y);
            ss << (cell->is_passable() ? 1 : 0) << " ";
            ss << (cell->is_trap() ? 1 : 0) << " ";
            ss << cell->get_trap_damage_value() << " ";
        }
        ss << "\n";
    }

    if (enemy_tower) {
        ss << "1 " << enemy_tower->get_x() << " " << enemy_tower->get_y() << " " << enemy_tower->hp << "\n";
    } else {
        ss << "0\n";
    }

    ss << enemies.size() << "\n";
    for (const auto& e : enemies) {
        // X Y HP Damage
        ss << e->x_pos << " " << e->y_pos << " " << e->hp << " " << e->get_damage() << "\n";
    }

    Hand* hand = player->get_hand();
    ss << hand->max_size() << "\n";

    const auto& spells = hand->get_spells();
    ss << spells.size() << "\n";

    for (const auto& spell : spells) {
        ss << spell->get_type_id() << " "
            << spell->name() << " "
            << spell->radius() << " "
            << spell->get_power() << "\n";
    }


    std::ofstream ofs(SAVE_FILE_NAME);
    if (!ofs) {
        throw std::runtime_error("Cannot open save file for writing.");
    }

    std::string data = ss.str();
    unsigned long long hash = calculate_hash(data); // Хэш

    ofs << hash << "\n";
    ofs << data;

    if (!ofs.good()) {
        throw std::runtime_error("Write error while saving.");
    }

    ofs.close();
}

void Game::load_gamefile() {
    std::ifstream ifs(SAVE_FILE_NAME);
    if (!ifs) {
        throw std::runtime_error("Save file does not exist.");
    }

    // Считываем хеш
    unsigned long long stored_hash;
    if (!(ifs >> stored_hash)) {
        throw std::runtime_error("Save file is empty or corrupted.");
    }

    char pop; // /n
    ifs.get(pop);

    // Считываем остальное содержимое
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    std::string data = buffer.str();
    ifs.close();

    if (calculate_hash(data) != stored_hash) {
        throw std::runtime_error("Save file modified.");
    }

    std::stringstream ss(data);

    // для безопасного чтения, если формат не совпадает ss.fail() -> true
    auto check_stream = [&ss]() {
        if (ss.fail()) throw std::runtime_error("Data format error in save file.");
    };

    int level, php, pmax, pscore, px, py;
    ss >> level >> php >> pmax >> pscore >> px >> py;
    check_stream();
    reset_game_state();
    current_level = level;
    int width, length;
    ss >> width >> length;
    check_stream();

    field = std::make_unique<Gamefield>(length, width);

    for (int y = 0; y < width; ++y) {
        for (int x = 0; x < length; ++x) {
            int is_passable, has_trap, trap_dmg;
            ss >> is_passable >> has_trap >> trap_dmg;
            check_stream();

            Cell* cell = field->get_cell(x, y);
            if (cell) {
                cell->set_passable(is_passable == 1);
                if (has_trap == 1) {
                    cell->place_trap(trap_dmg);
                }
            }
        }
    }

    if (!player) {
        player = std::make_unique<Player>(pmax, 0, 0, 5);
    }
    player->set_max_hp(pmax);
    player->hp = php;
    player->edit_score(pscore - player->get_score());

    Cell* player_cell = field->get_cell(px, py);
    if (!player_cell) throw std::runtime_error("Player out of bounds!");
    player->go_to(*player_cell);

    int has_tower;
    ss >> has_tower;
    check_stream();

    if (has_tower == 1) {
        int tx, ty, thp;
        ss >> tx >> ty >> thp;
        check_stream();
        Cell* tCell = field->get_cell(tx, ty);
        if (tCell) {
            enemy_tower = std::make_unique<EnemyTower>(tCell, 3);
            enemy_tower->hp = thp;
        }
    }

    size_t enemies_count;
    ss >> enemies_count;
    check_stream();

    for (size_t i = 0; i < enemies_count; ++i) {
        int ex, ey, ehp, edmg;
        ss >> ex >> ey >> ehp >> edmg;
        check_stream();

        Cell* eCell = field->get_cell(ex, ey);
        if (eCell) {
            auto e = std::make_unique<Enemy>(50, edmg, ex, ey);
            e->hp = ehp;
            e->go_to(*eCell);
            enemies.push_back(std::move(e));
        }
    }

    int hand_capacity;
    ss >> hand_capacity;
    check_stream();

    player->reset_hand(hand_capacity);
    size_t spells_count;
    ss >> spells_count;
    check_stream();

    Hand* hand = player->get_hand();
    for (size_t i = 0; i < spells_count; ++i) {
        int id;
        std::string s_name;
        int s_radius, s_power;

        ss >> id >> s_name >> s_radius >> s_power;
        check_stream();

        std::unique_ptr<Spell> new_spell = nullptr;
        if (id == 1) { new_spell = std::make_unique<DirectAttack>(s_name, s_radius, s_power); }
        else if (id == 2) { new_spell = std::make_unique<AOEAttack>(s_name, s_radius, s_power); }
        else if (id == 3) { new_spell = std::make_unique<Trap>(s_name, s_radius, s_power); }

        if (new_spell) { hand->add_spell(std::move(new_spell)); }
    }

    current_state = GameState::PLAYING;
}

Game::~Game() {
}