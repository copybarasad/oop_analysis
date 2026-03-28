#include "Game.h"
#include "IGameView.h"
#include "SaveLoadException.h"

void Game::set_view(IGameView* new_view) {
    view = new_view;
}

void Game::start_new_game() {
    if (view) view->show_message("Starting New Game...");

    current_level = 1;
    game_running = true;
    init_level();
}

void Game::init_level() {
    int width = 5 + current_level * 2;
    int height = 5 + current_level * 2;

    delete gamearea;
    gamearea = new GameArea(height, width);

    Coords player_coords(1, 1);
    if (!player) {
        player = new Player(100, 20, player_coords, 3);
    }
    else {
        player->healing();
        player->set_coords(player_coords.x, player_coords.y);
    }

    for (auto e : enemies) delete e;
    enemies.clear();
    for (auto s : summons) delete s;
    summons.clear();
    for (auto b : buildings) delete b;
    buildings.clear();

    int enemy_count = 5 + current_level;
    double enemy_health = 10 + current_level * 5;
    double enemy_damage = 5 + current_level * 2;

    for (int i = 0; i < enemy_count; ++i) {
        int x = rand() % width;
        int y = rand() % height;
        enemies.push_back(new Enemy(enemy_health, enemy_damage, Coords(x, y)));
    }

    buildings.push_back(new Tower(50, Coords(rand() % width, rand() % height)));

    if (current_level >= 2) {
        buildings.push_back(new EnemyBuilding(30, Coords(rand() % width, rand() % height), 3));
    }

    std::vector<Enemy*> enemy_ptrs = enemies;
    std::vector<Building*> building_ptrs = buildings;
    gamearea->fill_area(player, enemy_ptrs, building_ptrs);

    if (view) view->on_level_init(current_level);
}

void Game::move_player(std::string dir) {
    if (player && game_running) {
        player->moving(dir, *gamearea);
    }
}

void Game::cast_spell_mode() {
    if (player && game_running) {
        player->use_spell(player->get_coords(), *gamearea);
    }
}

void Game::save_game_command() {
    try {
        save_current_game();
        if (view) view->show_message("Game saved successfully.");
    }
    catch (const SaveLoadException& e) {
        if (view) view->show_message(std::string("Save failed: ") + e.what());
    }
}

void Game::stop_game() {
    game_running = false;
    if (view) view->show_message("Game stopped.");
}

bool Game::is_running() const {
    return game_running;
}

GameArea* Game::get_area() {
    return gamearea;
}

Player* Game::get_player() {
    return player;
}

void Game::update_world() {
    if (check_lose_condition()) {
        if (view) view->on_game_over(false);
        handle_game_over();
        return;
    }

    check_allies_on_area();
    allies_turn();
    enemies_turn();
    buildings_turn();

    if (check_lose_condition()) {
        if (view) view->on_game_over(false);
        handle_game_over();
        return;
    }

    if (check_win_condition()) {
        if (view) view->on_game_over(true);
        next_level();
    }
}

void Game::allies_turn() {
    for (auto& summon : summons) {
        if (summon->get_health() > 0) {
            summon->generate_move(*gamearea);
        }
    }
}

void Game::enemies_turn() {
    for (auto& enemy : enemies) {
        if (enemy->get_health() > 0) {
            enemy->generate_move(*gamearea);
        }
    }
}

void Game::buildings_turn() {
    for (auto& building : buildings) {
        if (building->get_health() <= 0) continue;
        building->do_turn(*gamearea);
    }
}

void Game::check_allies_on_area() {
    std::vector<Summon*> allies = gamearea->find_allies();
    for (auto new_summon : allies) {
        bool found = false;
        for (auto summon : summons) {
            if (new_summon == summon) {
                found = true;
                break;
            }
        }
        if (!found) {
            summons.push_back(new_summon);
        }
    }
}

bool Game::check_win_condition() {
    bool all_enemies_dead = true;
    for (const auto& enemy : enemies) {
        if (enemy->get_health() > 0) {
            all_enemies_dead = false;
            break;
        }
    }

    bool all_buildings_destroyed = true;
    for (const auto& building : buildings) {
        if (building->get_health() > 0) {
            all_buildings_destroyed = false;
            break;
        }
    }

    return all_enemies_dead && all_buildings_destroyed;
}

bool Game::check_lose_condition() {
    return player->get_health() <= 0;
}

void Game::handle_game_over() {
    if (!view) {
        game_running = false;
        return;
    }

    int choice = view->ask_game_over_action();

    switch (choice) {
    case 1:
        start_new_game();
        break;
    case 2:
        try {
            load_saved_game();
        }
        catch (const SaveLoadException& e) {
            view->show_message("Load failed. Starting new game.");
            start_new_game();
        }
        break;
    case 3:
    default:
        stop_game();
        break;
    }
}

void Game::upgrade_player() {
    int choice = view->ask_upgrade_choice();

    switch (choice) {
    case 1:
        player->upgrade_max_health(20);
        view->show_message("Health increased (+20)!");
        break;
    case 2:
        player->upgrade_damage(10);
        view->show_message("Damage increased (+10)!");
        break;
    case 3:
        player->add_slot_hand();
        view->show_message("Spell slot added!");
        break;
    default:
        view->show_message("Invalid choice. No upgrade applied.");
    }
}

void Game::next_level() {
    current_level++;
    if (view) view->show_message("Half of your spells were removed due to fatigue...");

    player->clear_half_hand();
    upgrade_player();
    init_level();
}

void Game::save_current_game() {
    GameState state;
    state.current_level = current_level;
    state.area_width = 5 + current_level * 2;
    state.area_height = 5 + current_level * 2;

    state.player_health = player->get_health();
    state.player_damage = player->get_damage();
    state.player_experience = player->get_exp();
    state.player_coords = player->get_coords();
    state.player_hand_size = 3;

    for (const auto& enemy : enemies) {
        if (enemy->get_health() > 0) {
            GameState::EnemyData ed;
            ed.health = enemy->get_health();
            ed.damage = enemy->get_damage();
            ed.coords = enemy->get_coords();
            state.enemies.push_back(ed);
        }
    }

    for (const auto& summon : summons) {
        if (summon->get_health() > 0) {
            GameState::SummonData sm;
            sm.health = summon->get_health();
            sm.damage = summon->get_damage();
            sm.coords = summon->get_coords();
            state.summons.push_back(sm);
        }
    }

    for (const auto& building : buildings) {
        if (building->get_health() > 0) {
            GameState::BuildingData bd;
            bd.health = building->get_health();
            bd.coords = building->get_coords();
            bd.spawn_step = 0;

            if (dynamic_cast<Tower*>(building)) {
                bd.type = "Tower";
            }
            else if (dynamic_cast<EnemyBuilding*>(building)) {
                bd.type = "EnemyBuilding";
            }
            else {
                bd.type = "Unknown";
            }

            state.buildings.push_back(bd);
        }
    }

    state.cell_types = gamearea->serialize_cell_types();
    SaveManager::save_game(state);
}

void Game::load_saved_game() {
    GameState state = SaveManager::load_game();

    current_level = state.current_level;

    delete gamearea;
    gamearea = new GameArea(state.area_height, state.area_width);

    delete player;
    player = new Player(state.player_health, state.player_damage,
        state.player_coords, state.player_hand_size);
    player->up_exp(state.player_experience);

    for (auto e : enemies) delete e;
    enemies.clear();
    for (auto s : summons) delete s;
    summons.clear();
    for (auto b : buildings) delete b;
    buildings.clear();

    for (const auto& ed : state.enemies) {
        enemies.push_back(new Enemy(ed.health, ed.damage, ed.coords));
    }

    for (const auto& sm : state.summons) {
        summons.push_back(new Summon(sm.health, sm.damage, sm.coords));
    }

    for (const auto& bd : state.buildings) {
        if (bd.type == "Tower") {
            buildings.push_back(new Tower(bd.health, bd.coords));
        }
        else if (bd.type == "EnemyBuilding") {
            buildings.push_back(new EnemyBuilding(bd.health, bd.coords, 3));
        }
    }

    std::vector<Enemy*> enemy_ptrs = enemies;
    std::vector<Building*> building_ptrs = buildings;
    gamearea->fill_area(player, enemy_ptrs, building_ptrs);
    gamearea->deserialize_cell_types(state.cell_types);

    game_running = true;
    if (view) view->show_message("Game loaded successfully!");
}
