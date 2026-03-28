#include "game.h"
#include "field.h"
#include "player.h"
#include "ally.h" 
#include "enemy_tower.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <random>
#include <vector>
#include "json.hpp"

Game::Game() : current_level_(0), game_over_(false), is_loaded_(false), player_(std::make_shared<Player>()) {
    InitializeLevels();
    input_handler_.LoadControls();
}

void Game::InitializeLevels() {
    levels_ = { 
        {20, 20, 8, 5, 5, 1, 50, 10, 80, 10, 50, 15},
        {25, 25, 7, 7, 7, 2, 70, 15, 100, 15, 70, 20},
        {30, 30, 6, 10, 10, 3, 100, 20, 120, 20, 90, 25}
    };
}

std::unique_ptr<Field> Game::CreateFieldFromConfig(int width, int height, int turns_to_spawn_enemy, int obstacle_count, int slow_count, int building_count) const {
    return std::make_unique<Field>(width, height, turns_to_spawn_enemy, obstacle_count, slow_count, building_count, player_);
}

void Game::StartGame() {
    std::cout << "Welcome to The Game\n";
    std::cout << "1. New Game\n2. Load Game\nChoose: ";
    int choice;
    std::cin >> choice;
    if (choice == 2) {
        try { LoadGame(); } catch (const FileException& e) {
            std::cout << "Load failed: " << e.what() << ". Starting new.\n";
        }
    }
    while (!game_over_ && current_level_ < levels_.size()) {
        RunLevel(current_level_);
        if (game_over_) break;
        if (IsLevelCompleted(*current_field_)) {
            std::cout << "Level " << (current_level_ + 1) << " completed!\n";
            NextLevel();
            UpgradePlayer();
        } else {
            // Игрок проиграл
            std::cout << "Game Over! 1. Restart 2. Exit\n";
            std::cin >> choice;
            if (choice == 1) {
                game_over_ = false;
                is_loaded_ = false;
                current_level_ = 0;
                player_ = std::make_shared<Player>();
            } else {
                game_over_ = true;
            }
        }
    }
    if (current_level_ >= levels_.size()) {
        std::cout << "All levels completed! Congratulations!\n";
    }
}

void Game::RunLevel(int level_index) {
    if (!is_loaded_) {
        auto& lvl = levels_[level_index];
        current_field_ = std::make_unique<Field>(
            lvl.width, lvl.height, lvl.turns_to_spawn_enemy,
            lvl.obstacle_count, lvl.slow_count, lvl.building_count,
            nullptr 
        );
        int px = current_field_->GetWidth() / 2;
        int py = current_field_->GetHeight() / 2;
        current_field_->SetPlayer(player_, px, py);
    }
    
    GameController<ConsoleInputHandler> controller(current_field_.get(), current_field_->GetPlayer());
    GameRenderer<ConsoleRenderer> renderer;
    
    bool save_requested = false;
    bool load_requested = false;
    bool quit_requested = false;
    bool loaded_this_turn = false;
    while (!IsLevelCompleted(*current_field_) && current_field_->IsPlayerAlive()) {
        renderer.RenderFullGameState(*current_field_);
        
        if (current_field_->IsPlayerSlowed()) {
            renderer.RenderMessage("You are slowed! Move commands ignored this turn.");
            current_field_->ResetPlayerSlowed();
        } else {
            std::cout << "Enter command: ";
            std::string command;
            std::getline(std::cin, command);
            
            auto result = controller.ProcessInput(command);
            
            if (result.action == GameController<ConsoleInputHandler>::ProcessResult::Action::Save) {
                save_requested = true;
            } else if (result.action == GameController<ConsoleInputHandler>::ProcessResult::Action::Load) {
                load_requested = true;
            } else if (result.action == GameController<ConsoleInputHandler>::ProcessResult::Action::Quit) {
                quit_requested = true;
            } else if (result.action == GameController<ConsoleInputHandler>::ProcessResult::Action::Invalid) {
                renderer.RenderMessage("Unknown command. Type 'help' for available commands.");
            } 
            
            if (result.turn_used && result.action == GameController<ConsoleInputHandler>::ProcessResult::Action::Continue) {
                current_field_->UpdateAllies();
                
                current_field_->MoveEnemies();
                
                current_field_->ProcessBuildings();
                current_field_->UpdateTowers();
            }
        }
        
        if (save_requested) {
            try {
                SaveGame();
                renderer.RenderMessage("Game saved!");
            } catch (const FileException& e) {
                renderer.RenderMessage("Save failed: " + std::string(e.what()));
            }
            save_requested = false;
        }
        
        if (load_requested) {
            try {
                LoadGame();
                renderer.RenderMessage("Game loaded!");
                loaded_this_turn = true;
            } catch (const FileException& e) {
                renderer.RenderMessage("Load failed: " + std::string(e.what()));
                loaded_this_turn = false;
            }
            load_requested = false;
            if (!IsLevelCompleted(*current_field_) && !current_field_->IsPlayerAlive()) {
                game_over_ = true;
                return;
            }
            if (loaded_this_turn) {
                controller = GameController<ConsoleInputHandler>(current_field_.get(), current_field_->GetPlayer());
                renderer = GameRenderer<ConsoleRenderer>();
                loaded_this_turn = false;
            }
        }
        
        if (quit_requested) {
            game_over_ = true;
            break;
        }
    }
    
    if (!current_field_->IsPlayerAlive()) {
        game_over_ = true;
    }
}


bool Game::IsLevelCompleted(const Field& field) const {
    return field.GetEnemies().empty(); 
}

void Game::NextLevel() {
    current_level_++;
    player_->HealToMax();
    auto& hand = player_->GetHand();
    int size = hand.GetSize();
    std::vector<int> indices(size);
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), std::mt19937{std::random_device{}()});
    int to_remove = size / 2;
    for (int i = 0; i < to_remove; ++i) {
        hand.RemoveSpell(indices[i]);
    }
    is_loaded_ = false;
}

void Game::UpgradePlayer() {
    std::cout << "Upgrade Player!\n1. +Health (+20 max HP, heal +20)\n2. +Damage (+5)\nChoose: ";
    int choice;
    std::cin >> choice;
    if (choice == 1) {
        player_->UpgradeHealth(20);
    } else if (choice == 2) {
        player_->UpgradeDamage(5);
    }
}

void Game::SaveGame() const {
    nlohmann::json j = Serialize();
    std::ofstream file("save.json", std::ios::out);
    if (!file) throw SaveFailedException("Cannot open file for writing");
    try {
        file << j.dump(4); // Красивый JSON )))
    } catch (...) {
        file.close();
        throw SaveFailedException("Failed to write JSON");
    }
    file.close();
}

void Game::LoadGame() {
    if (!std::filesystem::exists("save.json")) throw FileNotFoundException("save.json");
    std::ifstream file("save.json", std::ios::in);
    if (!file) throw FileNotFoundException("save.json");
    nlohmann::json j;
    try {
        file >> j;
    } catch (...) {
        file.close();
        throw InvalidSaveException("Invalid JSON format");
    }
    file.close();
    Deserialize(j);
    is_loaded_ = true;
}

nlohmann::json Game::Serialize() const {
    nlohmann::json j;
    j["current_level"] = current_level_;

    // Игрок
    j["player"] = {
        {"health", player_->GetHealth()},
        {"max_health", player_->GetMaxHealth()},
        {"points", player_->GetPoints()},
        {"base_damage", player_->GetBaseDamage()},
        {"kill_count", player_->GetKillCount()},
        {"fight_mode", player_->GetFightModeName()},
        {"buff_range", player_->GetBuffRange()},
        {"buff_area", player_->GetBuffArea()},
        {"buff_trap_damage", player_->GetBuffTrapDamage()},
        {"buff_summons", player_->GetBuffSummons()},
        {"hand", player_->GetHand().GetSpellTypes()}
    };


    if (current_field_) {
        j["field"] = {
            {"width", current_field_->GetWidth()},
            {"height", current_field_->GetHeight()},
            {"turn_count", current_field_->GetTurnCount()},
            {"player_x", current_field_->GetPlayerX()},
            {"player_y", current_field_->GetPlayerY()},
            {"player_slowed", current_field_->IsPlayerSlowed()},
            {"turns_to_spawn_enemy", current_field_->GetTurnsToSpawnEnemy()} 
        };

        j["field"]["grid"] = nlohmann::json::array();
        for (int y = 0; y < current_field_->GetHeight(); ++y) {
            nlohmann::json row = nlohmann::json::array();
            for (int x = 0; x < current_field_->GetWidth(); ++x) {
                int type = static_cast<int>(current_field_->GetGrid()[y][x].GetType()); 
                row.push_back(type);
            }
            j["field"]["grid"].push_back(row);
        }

        j["field"]["enemies"] = nlohmann::json::array();
        for (const auto& e : current_field_->GetEnemies()) {
            j["field"]["enemies"].push_back({
                {"x", e->GetX()},
                {"y", e->GetY()},
                {"health", e->GetHealth()},
                {"damage", e->GetDamage()}
            });
        }


        j["field"]["buildings"] = nlohmann::json::array();
        for (const auto& b : current_field_->GetBuildings()) {
            j["field"]["buildings"].push_back({
                {"x", b->GetX()},
                {"y", b->GetY()},
                {"health", b->GetHealth()},
                {"damage", b->GetDamage()}
            });
        }

        j["field"]["towers"] = nlohmann::json::array();
        for (const auto& t : current_field_->GetTowers()) {
            j["field"]["towers"].push_back({
                {"x", t->GetX()},
                {"y", t->GetY()},
                {"health", t->GetHealth()},
                {"damage", t->GetDamage()},
                {"cooldown", t->GetCooldown()}
            });
        }

        j["field"]["allies"] = nlohmann::json::array();
        for (const auto& a : current_field_->GetAllies()) {
            nlohmann::json ally_obj = {
                {"x", a->GetX()},
                {"y", a->GetY()},
                {"health", a->GetHealth()},
                {"damage", a->GetDamage()}
            };
            j["field"]["allies"].push_back(ally_obj);
        }

        j["field"]["traps"] = nlohmann::json::array();
        for (const auto& tr : current_field_->GetTraps()) {
            j["field"]["traps"].push_back({
                {"x", tr->GetX()},
                {"y", tr->GetY()},
                {"damage", tr->GetDamage()}
            });
        }
    }

    return j;
}

void Game::Deserialize(const nlohmann::json& j) {
    current_level_ = j["current_level"];

    player_ = std::make_shared<Player>();
    player_->SetHealth(j["player"]["health"]);
    player_->SetMaxHealth(j["player"]["max_health"]);
    player_->SetPoints(j["player"]["points"]);
    player_->SetBaseDamage(j["player"]["base_damage"]);
    player_->SetKillCount(j["player"]["kill_count"]);
    player_->SetFightMode(j["player"]["fight_mode"]);
    player_->SetBuffRange(j["player"]["buff_range"]);
    player_->SetBuffArea(j["player"]["buff_area"]);
    player_->SetBuffTrapDamage(j["player"]["buff_trap_damage"]);
    player_->SetBuffSummons(j["player"]["buff_summons"]);
    player_->GetHand().LoadFromTypes(j["player"]["hand"]);

    if (j.contains("field")) {
        int width = j["field"]["width"];
        int height = j["field"]["height"];
        int turns_to_spawn = j["field"]["turns_to_spawn_enemy"];
        current_field_ = std::make_unique<Field>(width, height, turns_to_spawn, 0, 0, 0, nullptr);
    
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                current_field_->GetGrid()[y][x].SetType(static_cast<CellType>(j["field"]["grid"][y][x]));
            }
        }
    
        current_field_->SetPlayerX(j["field"]["player_x"]);
        current_field_->SetPlayerY(j["field"]["player_y"]);
        current_field_->SetPlayerSlowed(j["field"]["player_slowed"]);
        current_field_->SetTurnCount(j["field"]["turn_count"]);
        current_field_->SetPlayer(player_, j["field"]["player_x"], j["field"]["player_y"]);
    
        std::vector<std::pair<int, int>> enemy_pos;
        std::vector<CellType> enemy_under_types;
        std::vector<bool> enemy_slowed;
        for (const auto& e : j["field"]["enemies"]) {
            int x = e["x"], y = e["y"], h = e["health"], d = e["damage"];
            current_field_->GetEnemies().emplace_back(std::make_unique<Enemy>(h, d, x, y));
            enemy_pos.emplace_back(x, y);
            enemy_under_types.push_back(current_field_->GetGrid()[y][x].GetType());
            enemy_slowed.push_back(false);
        }
        current_field_->SetEnemiesPositions(enemy_pos, enemy_under_types, enemy_slowed);
    
        std::vector<std::pair<int, int>> building_pos;
        for (const auto& b : j["field"]["buildings"]) {
            int x = b["x"], y = b["y"], h = b["health"], d = b["damage"];
            current_field_->GetBuildings().emplace_back(std::make_unique<Building>(h, d, x, y));
            building_pos.emplace_back(x, y);
        }
        current_field_->SetBuildingPositions(building_pos);
    
        std::vector<std::pair<int, int>> tower_pos;
        std::vector<CellType> tower_under_types;
        for (const auto& t : j["field"]["towers"]) {
            int x = t["x"], y = t["y"], h = t["health"], d = t["damage"], c = t["cooldown"];
            current_field_->GetTowers().emplace_back(std::make_unique<EnemyTower>(x, y, 3, d));
            auto tower = current_field_->GetTowers().back().get();
            tower->SetHealth(h);
            tower->SetCooldown(c);
            tower_pos.emplace_back(x, y);
            tower_under_types.push_back(current_field_->GetGrid()[y][x].GetType());
        }
        current_field_->SetTowerPositions(tower_pos, tower_under_types);

        std::vector<std::pair<int, int>> ally_pos;
        std::vector<CellType> ally_under_types;
        for (const auto& a : j["field"]["allies"]) {
            int x = a["x"], y = a["y"];
            current_field_->AddAlly(x, y);
            ally_pos.emplace_back(x, y);
            ally_under_types.push_back(current_field_->GetGrid()[y][x].GetType());  
        }
        current_field_->SetAllyPositions(ally_pos, ally_under_types);
    
        std::vector<std::pair<int, int>> trap_pos;
        std::vector<CellType> trap_under_types;
        for (const auto& tr : j["field"]["traps"]) {
            int x = tr["x"], y = tr["y"], dmg = tr["damage"];
            current_field_->AddTrap(x, y, dmg);
            trap_pos.emplace_back(x, y);
            trap_under_types.push_back(current_field_->GetGrid()[y][x].GetType());
        }
        current_field_->SetTrapPositions(trap_pos, trap_under_types);
    }

    is_loaded_ = true;  
}
