#include "game.hpp"
#include "field/point.hpp"
#include "entities/enemy.hpp"
#include "entities/ally.hpp"
#include "entities/enemy_building.hpp"
#include "entities/enemy_tower.hpp"
#include "file_open_exception.hpp"
#include "game/loger.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <termios.h> 
#include <unistd.h>
#include <nlohmann/json.hpp>
#include "input_reader.hpp"
#include "game_controller.hpp"

using json = nlohmann::json;

Game::~Game() {
    delete visualizer;
    delete controller;
    delete logger;
    delete current_level;
    delete player;
    delete hand;
    delete draw;
    delete field;
}

Point Game::rand_pos() {
    Point pos = {rand() % width, rand() % height};
    while(field->get_cell(pos).get_type() != CellType::Default || field->get_cell(pos).has_entity()) {
        pos = {rand() % width, rand() % height};
    }
    return pos;
}

struct termios original_termios;

void restore_termios_callback() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}

void Game::restore_termios_impl() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}

bool Game::prompt_load_or_new() {
    std::ifstream file("savegame.json");
    if (file.good()) {
        file.close();
        std::cout << "Found saved game. Load? (y/n): ";
        char choice;
        std::cin >> choice;
        std::cin.ignore();
        return (choice == 'y' || choice == 'Y');
    }
    return false;
}

void Game::calculate_field_size() {
    int steps = level_number - 1;
    width = 10 + steps / 2;
    height = 10 + (steps + 1) / 2;
    
    if (width > 25) width = 25;
    if (height > 25) height = 25;
}

void Game::init_game() {
    srand(time(nullptr));
    
    if (prompt_load_or_new()) {
        bool result = load_game();
        if (result) return;
        std::cout << "Failed to load save. Starting new game\n";
    }
    
    calculate_field_size();
    field = new Field(width, height);
    player = new Player(*field, rand_pos());
    field->get_cell(player->get_position()).set_entity(player);
    hand = new Hand(*field, *player);
    draw = new Draw(*field, cursor, *hand, *player);
    
    controller = new GameController<InputReader>();
    visualizer = new GameVisualizer<Draw>(*draw);
    current_level = new Level(*field, level_number);
    logger = new Loger(LogMode::Terminal);
    current_level->add_entity(player);
    current_level->init_entities(width, height);
    
    save_level_checkpoint();
    tcgetattr(STDIN_FILENO, &original_termios);
    struct termios raw_termios = original_termios;
    raw_termios.c_lflag &= ~(ICANON | ECHO | ISIG);
    raw_termios.c_cc[VMIN] = 1;
    raw_termios.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_termios);
    atexit(restore_termios_callback);
}

void Game::next_level() {
    level_number++;
    
    if (level_number > 31) {
        std::cout << "\033[92m\033[1m=== ПОБЕДА! ВЫ ПРОШЛИ ВСЕ УРОВНИ! ===\033[0m" << std::endl;
        return;
    }
    
    int old_score = player->get_score();
    int old_melee_damage = player->get_melee_damage();
    int old_ranged_damage = player->get_ranged_damage();
    int old_ranged_radius = player->get_ranged_radius();
    AttackType old_attack_type = player->get_attack_type();
    
    player->set_health(99);
    if (level_number % 2 == 0) {
        player->upgrade_damage();
    }
    hand->level_up_spells();
    hand->level_up_base_spells();
    hand->remove_half_spells();
    
    int new_melee_damage = player->get_melee_damage();
    int new_ranged_damage = player->get_ranged_damage();
    int new_ranged_radius = player->get_ranged_radius();
    
    for (Entity* e : current_level->get_entities()) {
        if (e->get_entity_type() != EntityType::Player) {
            field->get_cell(e->get_position()).set_entity(nullptr);
        }
    }
    
    delete current_level;
    delete visualizer;
    delete draw;
    delete field;
    
    calculate_field_size();
    field = new Field(width, height);
    
    Point new_pos = rand_pos();
    Player* old_player = player;
    player = new Player(*field, new_pos, 99);
    field->get_cell(new_pos).set_entity(player);
    
    while (player->get_melee_damage() < new_melee_damage) {
        player->upgrade_damage();
    }
    player->add_score(old_score - player->get_score());
    if (old_attack_type != player->get_attack_type()) {
        player->switch_attack_type();
    }
    
    delete old_player;
    
    Hand* old_hand = hand;
    hand = new Hand(*field, *player, 5, 0);
    old_hand->transfer_spells_to(*hand);
    delete old_hand;
    
    draw = new Draw(*field, cursor, *hand, *player);
    visualizer = new GameVisualizer<Draw>(*draw);
    
    current_level = new Level(*field, level_number);
    current_level->add_entity(player);
    current_level->init_entities(width, height);
    
    save_level_checkpoint();
}

void Game::game_loop() {
    visualizer->draw_info();
    
    while (player->is_entity_alive()) {
        if (current_level->get_enemy_count() == 0) {
            std::cout << "\033[92mLevel " << level_number << " completed!\033[0m" << std::endl;
            next_level();
            if (level_number > 31) {
                return;
            }
            std::cout << "\033[93mLevel " << level_number << " started! Field size: " 
                      << width << "x" << height << "\033[0m" << std::endl;
            continue;
        }
        
        visualizer->draw_all();
        controller->process_input(this);
    }
    
    visualizer->draw_field();
    std::cout << "\033[91mGAME OVER\033[0m" << std::endl;
    std::cout << "Reached level: " << level_number << std::endl;
    std::cout << "Retry level? (y/n): ";
    char choice = fgetc(stdin);
    std::cout << std::endl;
    
    if (choice == 'y' || choice == 'Y') {
        load_level_checkpoint();
        game_loop();
    }
}

void Game::set_cursor_pos() {
    cursor.is_enabled = true;
    bool cursor_active = true; 
    
    while (cursor_active) { 
        visualizer->draw_all();
        Command cursor_cmd = controller->get_next_command();
        Command confirm_cmd = controller->get_confirm_command();
        if (cursor_cmd == Command::Up) {
            if (cursor.position.y > 0) cursor.position.y--;
        } else if (cursor_cmd == Command::Down) {
            if (cursor.position.y < height - 1) cursor.position.y++;
        } else if (cursor_cmd == Command::Right) {
            if (cursor.position.x < width - 1) cursor.position.x++;
        } else if (cursor_cmd == Command::Left) {
            if (cursor.position.x > 0) cursor.position.x--;
        } else if (cursor_cmd == confirm_cmd) {
            cursor_active = false;  
        } else if (cursor_cmd == Command::Exit) {
            cursor_active = false;  
        }
    }
    cursor.is_enabled = false;
}

void Game::add_entity(Entity *entity) {
    current_level->add_entity(entity);
}

void Game::delete_entity(Entity *entity) {
    current_level->delete_entity(entity);
}

void Game::save_game() {
    try {
        nlohmann::json game_json;
        game_json["level_number"] = level_number;
        game_json["width"] = width;
        game_json["height"] = height;
        game_json["base_spell_level"] = Hand::base_spell_level;
        game_json["player"] = player->save();
        game_json["field"] = field->save();
        game_json["entities"] = current_level->save();
        game_json["hand"] = hand->save();

        std::ofstream file("savegame.json");
        if (!file.is_open()) {
            throw FileOpenException("savegame.json");
        }
        file << game_json.dump(2);
        file.close();
        
        std::cout << "\033[92mGame saved\033[0m" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "\033[91mSave failed: " << e.what() << "\033[0m" << std::endl;
    }
}
     
bool Game::load_game() {
    try {
        std::ifstream file("savegame.json");
        if (!file.is_open()) {
            throw FileOpenException("savegame.json");
        }
        
        nlohmann::json game_json;
        file >> game_json;
        file.close();
        
        level_number = game_json["level_number"];
        width = game_json["width"];
        height = game_json["height"];
        Hand::base_spell_level = game_json["base_spell_level"];
        
        field = new Field(width, height);
        
        auto field_json = game_json["field"];
        std::vector<int> cells = field_json["cell_types"];
        std::vector<int> traps = field_json["trap_data"];
        
        field->load_from_data(cells, traps);
        
        auto player_json = game_json["player"];
        Point player_pos = {player_json["x"], player_json["y"]};
        int player_health = player_json["health"];
    
        player = new Player(*field, player_pos, player_health);
        
        player->add_score(player_json["score"].get<int>() - 15);
        if (player_json["attack_type"] == 1) {
            player->switch_attack_type();
        }
        
        int target_melee = player_json.value("melee_damage", 3);
        while (player->get_melee_damage() < target_melee) {
            player->upgrade_damage();
        }
        
        field->get_cell(player->get_position()).set_entity(player);

        hand = new Hand(game_json["hand"], *field, *player, 5);
        draw = new Draw(*field, cursor, *hand, *player);
        controller = new GameController<InputReader>();
        visualizer = new GameVisualizer<Draw>(*draw);
        
        current_level = new Level(*field, level_number);
        current_level->add_entity(player);

        logger = new Loger(LogMode::Terminal);
        
        for (auto& entity_json : game_json["entities"]) {
            EntityType type = (EntityType)entity_json["type"].get<int>();
            Point pos = {entity_json["x"], entity_json["y"]};
            int health = entity_json["health"];
            
            Entity* entity = nullptr;
            if (type == EntityType::Enemy) {
                int damage = entity_json.value("damage", 2);
                entity = new Enemy(*field, pos, health, damage);
            } else if (type == EntityType::Ally) {
                entity = new Ally(*field, pos, health);
            } else if (type == EntityType::EnemyBuilding) {
                int enemy_hp = entity_json.value("enemy_hp", 12);
                int enemy_damage = entity_json.value("enemy_damage", 2);
                entity = new EnemyBuilding(*field, pos, 25, health, enemy_hp, enemy_damage);
            } else if (type == EntityType::EnemyTower) {
                entity = new EnemyTower(*field, pos, health);
            }
            
            if (entity) {
                field->get_cell(pos).set_entity(entity);
                current_level->add_entity(entity);
            }
        }
        
        tcgetattr(STDIN_FILENO, &original_termios);
        struct termios raw_termios = original_termios;
        raw_termios.c_lflag &= ~(ICANON | ECHO | ISIG);
        raw_termios.c_cc[VMIN] = 1;
        raw_termios.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_termios);
        atexit(restore_termios_callback);
        
        std::cout << "\033[92mGame loaded. Level " << level_number << "\033[0m" << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cout << "\033[91mLoad failed: " << e.what() << "\033[0m" << std::endl;
        
        delete current_level;
        delete visualizer;
        delete controller;
        delete logger;
        delete player;
        delete hand;
        delete draw;
        delete field;
        
        current_level = nullptr;
        visualizer = nullptr;
        controller = nullptr;
        logger = nullptr;
        player = nullptr;
        hand = nullptr;
        draw = nullptr;
        field = nullptr;
        
        return false;
    }
}

void Game::save_level_checkpoint() {
    try {
        nlohmann::json checkpoint_json;
        checkpoint_json["level_number"] = level_number;
        checkpoint_json["width"] = width;
        checkpoint_json["height"] = height;
        checkpoint_json["base_spell_level"] = Hand::base_spell_level;
        checkpoint_json["player"] = player->save();
        checkpoint_json["field"] = field->save();
        checkpoint_json["entities"] = current_level->save();
        checkpoint_json["hand"] = hand->save();
        
        std::ofstream file("level_checkpoint.json");
        if (!file.is_open()) {
            throw FileOpenException("level_checkpoint.json");
        }
        file << checkpoint_json.dump(2);
        file.close();
    } catch (const std::exception& e) {
        std::cout << "\033[91mCheckpoint save failed: " << e.what() << "\033[0m" << std::endl;
    }
}

void Game::load_level_checkpoint() {
    try {
        std::ifstream file("level_checkpoint.json");
        if (!file.is_open()) {
            throw FileOpenException("level_checkpoint.json");
        }
        
        nlohmann::json checkpoint_json;
        file >> checkpoint_json;
        file.close();
        
        delete current_level;
        delete visualizer;
        delete player;
        delete hand;
        delete draw;
        delete field;
        
        level_number = checkpoint_json["level_number"];
        width = checkpoint_json["width"];
        height = checkpoint_json["height"];
        Hand::base_spell_level = checkpoint_json["base_spell_level"];
        
        field = new Field(width, height);
        
        auto field_json = checkpoint_json["field"];
        std::vector<int> cells = field_json["cells"];
        std::vector<int> traps = field_json["traps"];
        field->load_from_data(cells, traps);
        
        auto player_json = checkpoint_json["player"];
        player = new Player(*field, {player_json["x"], player_json["y"]}, player_json["health"]);
        player->add_score(player_json["score"].get<int>() - 15);
        if (player_json["attack_type"] == 1) player->switch_attack_type();

        int target_melee = player_json.value("melee_damage", 3);
        while (player->get_melee_damage() < target_melee) {
            player->upgrade_damage();
        }
        
        field->get_cell(player->get_position()).set_entity(player);
        hand = new Hand(checkpoint_json["hand"], *field, *player, 5);
        draw = new Draw(*field, cursor, *hand, *player);
        
        visualizer = new GameVisualizer<Draw>(*draw);
        
        current_level = new Level(*field, level_number);
        current_level->add_entity(player);
        
        for (auto& entity_json : checkpoint_json["entities"]) {
            EntityType type = (EntityType)entity_json["type"].get<int>();
            Point pos = {entity_json["x"], entity_json["y"]};
            int health = entity_json["health"];
            
            Entity* entity = nullptr;
            if (type == EntityType::Enemy) {
                int damage = entity_json.value("damage", 2);
                entity = new Enemy(*field, pos, health, damage);
            } else if (type == EntityType::Ally) {
                entity = new Ally(*field, pos, health);
            } else if (type == EntityType::EnemyBuilding) {
                int enemy_hp = entity_json.value("enemy_hp", 12);
                int enemy_damage = entity_json.value("enemy_damage", 2);
                entity = new EnemyBuilding(*field, pos, 25, health, enemy_hp, enemy_damage);
            } else if (type == EntityType::EnemyTower) {
                entity = new EnemyTower(*field, pos, health);
            }
            
            if (entity) {
                field->get_cell(pos).set_entity(entity);
                current_level->add_entity(entity);
            }
        }
    } catch (const std::exception& e) {
        std::cout << "\033[91mCheckpoint load failed: " << e.what() << "\033[0m" << std::endl;
    }
}

void Game::player_move_up() {
    if (!cursor.is_enabled) {
        player->set_action(Action::up);  
        make_steps();
    }
}

void Game::player_move_down() {
    if (!cursor.is_enabled) {
        player->set_action(Action::down);
        make_steps();
    }
}

void Game::player_move_left() {
    if (!cursor.is_enabled) {
        player->set_action(Action::left);
        make_steps();
    }
}

void Game::player_move_right() {
    if (!cursor.is_enabled) {
        player->set_action(Action::right);
        make_steps();
    }
}

void Game::player_attack() {
    if (!cursor.is_enabled) {
        cursor.area_size = 1;
        cursor.radius = (player->get_attack_type() == AttackType::Melee) ? 1 : player->get_ranged_radius();
        cursor.position = player->rand_enemy();
        set_cursor_pos();
        player->set_action(Action::attack);  
        player->set_point(cursor.position);
        cursor.radius = 0;
        make_steps();
    }
}

void Game::player_switch_attack() {
    player->switch_attack_type();
}

void Game::show_hand() {
    hand->show_hand();
}

void Game::buy_spell() {
    if (cursor.is_enabled) return;
    
    int spell_idx = 0;
    bool in_shop_menu = true; 
    
    while (in_shop_menu) {
        visualizer->draw_all();
        hand->show_shop_str(spell_idx);
        hand->show_shop_spell_info(spell_idx);
        
        Command menu_cmd = controller->get_next_command();
        Command confirm_cmd = controller->get_confirm_command();
        
        if (menu_cmd == Command::Right && spell_idx < 5) {
            spell_idx++;
        } else if (menu_cmd == Command::Left && spell_idx > 0) {
            spell_idx--;
        } else if (menu_cmd == confirm_cmd) {
            hand->buy_spell(spell_idx + 1);
            in_shop_menu = false;  
        } else if (menu_cmd == Command::Exit) {
            in_shop_menu = false;  
        }
    }
}

void Game::use_spell() {
    if (cursor.is_enabled || hand->get_spell_count() == 0) return;
    
    int spell_idx = 0;
    bool in_spell_menu = true; 
    
    while (in_spell_menu) {
        visualizer->draw_all();
        hand->show_spells_str(spell_idx);
        hand->show_hand_spell_info(spell_idx);
        
        Command menu_cmd = controller->get_next_command();
        Command confirm_cmd = controller->get_confirm_command();
        
        if (menu_cmd == Command::Right && spell_idx < hand->get_spell_count() - 1) {
            spell_idx++;
        } else if (menu_cmd == Command::Left && spell_idx > 0) {
            spell_idx--;
        } else if (menu_cmd == confirm_cmd) {
            if (hand->prepare_spell(spell_idx)) {
                if (hand->spell_needs_cursor(spell_idx)) {
                    cursor.area_size = hand->get_spell_area_size(spell_idx);
                    cursor.radius = hand->get_spell_radius(spell_idx);
                    cursor.position = player->rand_enemy();
                    set_cursor_pos();
                }
                
                if (hand->use_spell(spell_idx, cursor.position)) {
                    std::vector<Entity*> new_allies = hand->get_last_pending_allies();
                    for (Entity* ally : new_allies) {
                        add_entity(ally);
                    }
                    cursor.area_size = 1;
                    cursor.radius = 0;
                    player->set_action(Action::skip_step);
                    make_steps();
                }
            }
            in_spell_menu = false; 
        } else if (menu_cmd == Command::Exit) {
            in_spell_menu = false; 
        }
    }
}

void Game::exit_game() {
    std::cout << "Are you sure you want to quit? (y/n): ";
    char confirm = fgetc(stdin);
    std::cout << std::endl;
    if (confirm == 'y' || confirm == 'Y') {
        restore_termios_impl();
        exit(0);
    }
}

void Game::load_game_cmd() {
    std::cout << "Load game? Current progress will be lost! (y/n): ";
    char confirm = fgetc(stdin);
    if (confirm == 'y' || confirm == 'Y') {
        delete current_level;
        delete visualizer;
        delete controller;  
        delete logger;      
        delete player;
        delete hand;
        delete draw;
        delete field;
        
        current_level = nullptr;
        visualizer = nullptr;
        controller = nullptr;
        logger = nullptr;
        player = nullptr;
        hand = nullptr;
        draw = nullptr;
        field = nullptr;
        
        if (!load_game()) {
            std::cout << "Failed to load game\n";
            return;
        }
    }
}

void Game::show_help() {
    visualizer->draw_info();
}

void Game::make_steps() {
    std::vector<Entity*>& entities = current_level->get_entities();
    for (size_t i = 0; i < entities.size(); i++) {
        if (entities[i]->is_entity_alive()) {
            logger->take_info(entities[i]->make_step());
        }
    }

    std::vector<Entity*> to_remove;
    std::vector<Entity*> to_add;
    
    for (Entity* e : current_level->get_entities()) {
        if (!e->is_entity_alive()) {
            to_remove.push_back(e);
        }
        Entity* new_entity = e->get_pending_entity();
        if (new_entity) to_add.push_back(new_entity);
    }
    
    for (Entity* e : to_remove) {
        delete_entity(e);
    }
    for (Entity* e : to_add) {
        add_entity(e);
    }
}