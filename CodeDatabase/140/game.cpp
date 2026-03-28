#include "game.h"
#include "commands.h"
#include "game_cycle.h"
#include "draw.h"
#include "input.h"
#include "keyboard_input.h"
#include "game_controller.h"
#include "game_view.h"

#include <iostream>
#include <limits>
#include <random>

#include "area_spell.h"
#include "direct_spell.h"
#include "trap_spell.h"
#include "create_spell.h"
#include "levelup_spell.h"

Game::Game() 
    : field(nullptr),
    running(true),
    command(nullptr) {}

Game::~Game() = default;

void Game::start_new_game() {
    int w = field ? field->get_width() : 15;
    int h = field ? field->get_height() : 15;

    field = std::make_unique<Field>(w, h);
    field->set_level(1);
    renderer = std::make_unique<Renderer>();
    command = std::make_unique<Command>(*field, running, *this, *renderer);
    input_handler = std::make_unique<InputHandler>(*command, running);

    field->create_player(1, 1, 100, 20, 10);

    field->create_building(field->get_width() - 4, field->get_height() - 4, 3, 50);
    field->create_tower(field->get_width() - 6, field->get_height() - 7, 10, 3, 5);

    field->create_enemy(5, 5, 40, 5);
    field->create_enemy(8, 3, 40, 5);
    field->create_enemy(3, 8, 40, 5);

    field->initialize_grid();
}

void Game::game_loop() {
    GameView<Renderer> view(*this);
    KeyboardInput kb;
    GameController<KeyboardInput> controller(*this, *command, kb);

    bool& runningRef = running;

    view.on_game_started();
    while (runningRef && !field->is_game_over()) {
        view.on_turn();
        controller.step();

        if (field->is_level_completed()) {
            show_upgrade_menu();
            advance_to_next_level();
            view.on_game_started();
        }
    }

    if (field->is_game_over()) {
        view.on_game_over();
    }
}

void Game::save_game() const {
    try {
        GameCycle::save(*field);
    }
    catch (const SaveLoadException& e) {
        cout << "SAVE ERROR: " << e.what() << '\n';
    }
    catch (...) {
        cout << "SAVE ERROR: Unknown error occurred.\n";
    }
}

void Game::load_game() {
    try {
        field = GameCycle::load();
        renderer = std::make_unique<Renderer>();
        command = std::make_unique<Command>(*field, running, *this, *renderer);
        input_handler = std::make_unique<InputHandler>(*command, running);

        cout << "Game loaded successfully!\n";
    }
    catch (const CorruptedSaveException& e) {
        cout << "LOAD ERROR: " << e.what() << "\nStarting new game.\n";
        start_new_game();
    }
    catch (const FileOpenException& e) {
        cout << "LOAD ERROR: " << e.what() << "\nStarting new game.\n";
        start_new_game();
    }
    catch (...) {
        cout << "LOAD ERROR: Unknown error. Starting new game.\n";
        start_new_game();
    }
}

void Game::game_over_screen(){
    if (renderer && field) {
        renderer->render_game_over(*field);
    } else if (field) {
        cout << "\n";
        cout << "               GAME OVER                \n";
        cout << "Final Score: " << field->get_player_score() << "\n";
    }
}

bool Game::ask_restart_or_exit() {
    char choice;
    while (true) {
        cout << "\nPlay again? (y/n): ";
        cin >> choice;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        choice = std::tolower(choice);
        if (choice == 'y') return true;
        if (choice == 'n') return false;

        cout << "Please enter 'y' or 'n'.\n";
    }
}

void Game::run() {
    int w = 15, h = 15;
    cout << "Enter field width (10-25): ";
    cin >> w;
    cout << "Enter field width (10-25): ";
    cin >> h;
    if (w < 10 || w > 25) w = 15;
    if (h < 10 || h > 25) h = 15;

    field = std::make_unique<Field>(w, h);

    char load_choice;
    cout << "Load saved game? (y/n): ";
    cin >> load_choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (std::tolower(load_choice) == 'y') {
        load_game();
    }else {
        start_new_game();
    }

    if (std::tolower(load_choice) != 'y' || !field->get_player()) {
        start_new_game();
    }

    while (running) {
        game_loop();

        if (!running) break;

        game_over_screen();

        if (ask_restart_or_exit()) {
            start_new_game();
        } else {
            running = false;
        }
    }
}

void Game::advance_to_next_level() {
    if (!field || !field->get_player()) return;

    std::cout << "\nLevel " << field->get_level() << " completed! Advancing to next level.\n";

    // Заклинания
    vector<string> spell_names;
    const auto& spells = field->get_player()->get_hand().get_spells();
    for (const auto& spell : spells) {
        spell_names.push_back(spell->get_name());
    }

    // Новые размеры поля (+2)
    int new_w = std::min(field->get_width() + 2, 25);
    int new_h = std::min(field->get_height() + 2, 25);

    // Создание нового поля
    field = std::make_unique<Field>(new_w, new_h);
    field->set_level(field->get_level() + 1);

    // Восстановление здоровья игрока
    int max_health = 100 + 20 * (field->get_level() - 1);
    int close_dmg = 20 + 5 * (field->get_level() - 1);
    int far_dmg = 10 + 2 * (field->get_level() - 1);
    field->create_player(1, 1, max_health, close_dmg, far_dmg);

    // Удаление половины заклинаний
    if (!spell_names.empty()) {
        for (size_t i = spell_names.size() - 1; i > 0; --i) {
            size_t j = static_cast<size_t>(std::rand()) % (i + 1);
            std::swap(spell_names[i], spell_names[j]);
        }
        size_t total = spell_names.size();
        size_t keep_count = total - total / 2;

        auto& hand = field->get_player()->get_hand();
        for (size_t i = 0; i < keep_count; ++i) {
            const std::string& name = spell_names[i];
            if (name == "Direct")      hand.add_spell(std::make_unique<DirectDamageSpell>());
            else if (name == "Area")   hand.add_spell(std::make_unique<AreaDamageSpell>());
            else if (name == "Trap")   hand.add_spell(std::make_unique<TrapSpell>());
            else if (name == "Create") hand.add_spell(std::make_unique<CreateSpell>());
            else if (name == "Level")  hand.add_spell(std::make_unique<LevelupSpell>());
        }
    }

    // Создание более сильных врагов и здания
    int enemy_count = 2 + field->get_level();
    int base_hp = 40 + 15 * field->get_level();
    int base_dmg = 5 + 2 * field->get_level();

    for (int i = 0; i < enemy_count && i < 10; ++i) {
        int x = 4 + (i * 2) % (new_w - 8);
        int y = 4 + (i / 3) % (new_h - 8);
        if (field->is_valid_position(x, y) && !field->is_cell_occupied(x, y)) {
            field->create_enemy(x, y, base_hp, base_dmg);
        }
    }

    // Здание
    int building_x = new_w - 4;
    int building_y = new_h - 4;
    if (field->is_valid_position(building_x, building_y) && !field->is_cell_occupied(building_x, building_y)) {
        field->create_building(building_x, building_y, std::max(1, 4 - field->get_level() / 3), 50 + 20 * field->get_level());
    }

    // Башня
    int tower_x = new_w - 6;
    int tower_y = new_h - 7;
    if (field->is_valid_position(tower_x, tower_y) && !field->is_cell_occupied(tower_x, tower_y)) {
        field->create_tower(tower_x, tower_y, 10 + 5 * field->get_level(), 3, 4);
    }

    field->initialize_grid();

    renderer = std::make_unique<Renderer>();
    command = std::make_unique<Command>(*field, running, *this, *renderer);
    input_handler = std::make_unique<InputHandler>(*command, running);

    cout << "New Level " << field->get_level() << "\n";
}

void Game::show_upgrade_menu() {
    if (!field || !field->get_player()) return;

    cout << "\nLevel Up! Choose an upgrade:\n";
    cout << "1) +20 max health\n";
    cout << "2) +5 melee damage\n";
    cout << "3) +3 ranged damage\n";

    const auto& spells = field->get_player()->get_hand().get_spells();
    for (size_t i = 0; i < spells.size(); ++i) {
        cout << (4 + i) << ") Upgrade spell: " << spells[i]->get_name() << "\n";
    }

    int choice;
    while (true) {
        cout << "Enter your choice: ";
        if (std::cin >> choice) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (choice == 1) {
                field->get_player()->upgrade_max_health();
                cout << "Health upgraded!\n";
                break;
            } else if (choice == 2) {
                field->get_player()->upgrade_close_damage();
                cout << "Melee damage upgraded!\n";
                break;
            } else if (choice == 3) {
                field->get_player()->upgrade_far_damage();
                cout << "Ranged damage upgraded!\n";
                break;
            } else if (choice >= 4 && choice < 4 + static_cast<int>(spells.size())) {
                size_t spell_index = static_cast<size_t>(choice - 4);
                spells[spell_index]->upgrade();
                cout << "Spell upgraded!\n";
                break;
            }
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        cout << "Invalid choice. Try again.\n";
    }
}