#include "Game.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "EnhancementSpell.h"
#include "GameException.h"
#include "Logger.h"
#include <fstream>
#include <random>
#include <algorithm>
#include <filesystem>

Game::Game()
    : field_(nullptr),
      player_(nullptr),
      player_hand_(nullptr),
      state_(GameState::MENU),
      current_level_(1),
      enemies_killed_(0),
      player_turn_ended_(false) {}

Game::~Game() = default;

void Game::StartNewGame() {
    current_level_ = 1;
    enemies_killed_ = 0;
    ClearLevel();

    int field_size = 15;
    field_ = std::make_unique<GameField>(field_size, field_size);

    int start_x = 2, start_y = 2;
    while (!field_->IsPassable(start_x, start_y)) {
        start_x++;
        if (start_x >= field_size - 2) {
            start_x = 2;
            start_y++;
        }
    }

    player_ = std::make_unique<Player>(start_x, start_y);
    field_->GetCell(start_x, start_y).SetPlayer(true);

    player_hand_ = std::make_unique<PlayerHand>(5);
    player_hand_->AddSpell(CreateRandomSpell());

    InitializeLevel();
    state_ = GameState::PLAYING;

    NotifyEvent(GameEvent(EventType::GAME_START, "New game started"));
}

void Game::PauseGame() {
    if (state_ == GameState::PLAYING) {
        state_ = GameState::PAUSE_MENU;
    }
}

void Game::ResumeGame() {
    if (state_ == GameState::PAUSE_MENU) {
        state_ = GameState::PLAYING;
    }
}

void Game::ProcessCommand(const Command& command) {
    if (!command.IsValid()) return;

    PlayerAction action = command.GetAction();

    switch (state_) {
        case GameState::MENU:
            if (action == PlayerAction::START_NEW_GAME) {
                StartNewGame();
            } else if (action == PlayerAction::LOAD_GAME) {
                // skip
            } else if (action == PlayerAction::QUIT_GAME) {
                // skip
            }
            break;

        case GameState::PLAYING:
            ProcessPlayerAction(action, command.GetSpellIndex(),
                              command.GetTargetX(), command.GetTargetY());
            break;

        case GameState::PAUSE_MENU:
            if (action == PlayerAction::RESUME_GAME) {
                ResumeGame();
            } else if (action == PlayerAction::START_NEW_GAME) {
                StartNewGame();
            } else if (action == PlayerAction::LOAD_GAME) {
                // skip
            }
            break;

        case GameState::LEVEL_COMPLETE:
            if (action == PlayerAction::START_NEW_GAME) {
                NextLevel();
            }
            break;

        case GameState::UPGRADE_MENU:
            if (action == PlayerAction::SELECT_UPGRADE_1) {
                UpgradePlayer(0);
            } else if (action == PlayerAction::SELECT_UPGRADE_2) {
                UpgradePlayer(1);
            } else if (action == PlayerAction::SELECT_UPGRADE_3) {
                UpgradePlayer(2);
            }
            break;

        case GameState::GAME_OVER:
            if (action == PlayerAction::START_NEW_GAME) {
                StartNewGame();
            }
            break;
    }
}

void Game::ProcessPlayerAction(PlayerAction action, int spell_index,
                                int target_x, int target_y) {
    if (state_ != GameState::PLAYING || player_turn_ended_) {
        return;
    }

    int old_x = player_->GetX();
    int old_y = player_->GetY();
    int new_x = old_x;
    int new_y = old_y;

    switch (action) {
        case PlayerAction::MOVE_UP:
            new_y = old_y - 1;
            break;
        case PlayerAction::MOVE_DOWN:
            new_y = old_y + 1;
            break;
        case PlayerAction::MOVE_LEFT:
            new_x = old_x - 1;
            break;
        case PlayerAction::MOVE_RIGHT:
            new_x = old_x + 1;
            break;
        case PlayerAction::SWITCH_COMBAT_MODE:
            player_->SwitchCombatMode();
            EndPlayerTurn();
            return;
        case PlayerAction::CAST_SPELL:
            if (spell_index >= 0 && spell_index < player_hand_->GetSpellCount()) {
                Spell* spell = player_hand_->GetSpell(spell_index);
                if (spell && spell->CanCast(old_x, old_y, target_x, target_y, *field_)) {
                    ApplyEnhancementToSpell(spell);
                    spell->Cast(old_x, old_y, target_x, target_y, *field_, *player_,
                               enemies_, enemy_buildings_, enemy_towers_, allies_);

                    NotifyEvent(GameEventFactory::SpellCast(spell->GetName(), target_x, target_y));

                    if (spell->GetType() != SpellType::ENHANCEMENT) {
                        player_hand_->ResetEnhancementStacks();
                    } else {
                        player_hand_->AddEnhancementStack();
                    }
                    player_hand_->RemoveSpell(spell_index);
                    EndPlayerTurn();
                }
            }
            return;
        case PlayerAction::SKIP_TURN:
            EndPlayerTurn();
            return;
        case PlayerAction::ATTACK: {
            int player_x = player_->GetX();
            int player_y = player_->GetY();
            CombatMode mode = player_->GetCombatMode();
            int attack_range = (mode == CombatMode::MELEE) ? 1 : 3;
            bool attacked = false;

            for (auto& enemy : enemies_) {
                if (!enemy->IsAlive()) continue;

                int enemy_x = enemy->GetX();
                int enemy_y = enemy->GetY();
                int distance = CalculateDistance(player_x, player_y, enemy_x, enemy_y);

                if (distance <= attack_range) {
                    enemy->TakeDamage(player_->GetDamage());
                    attacked = true;

                    NotifyEvent(GameEventFactory::PlayerAttack(enemy_x, enemy_y, player_->GetDamage()));

                    if (!enemy->IsAlive()) {
                        field_->GetCell(enemy_x, enemy_y).SetEnemy(false);
                        player_->AddScore(enemy->GetScoreValue());
                        player_->AddExperience(20);
                        enemies_killed_++;
                        NotifyEvent(GameEventFactory::EnemyDeath(enemy_x, enemy_y, enemy->GetScoreValue()));
                    }
                }
            }

            for (auto& building : enemy_buildings_) {
                if (!building->IsAlive()) continue;

                int building_x = building->GetX();
                int building_y = building->GetY();
                int distance = CalculateDistance(player_x, player_y, building_x, building_y);

                if (distance <= attack_range) {
                    building->TakeDamage(player_->GetDamage());
                    attacked = true;

                    if (!building->IsAlive()) {
                        field_->GetCell(building_x, building_y).SetEnemyBuilding(false);
                        player_->AddScore(50);
                    }
                }
            }

            for (auto& tower : enemy_towers_) {
                if (!tower->IsAlive()) continue;

                int tower_x = tower->GetX();
                int tower_y = tower->GetY();
                int distance = CalculateDistance(player_x, player_y, tower_x, tower_y);

                if (distance <= attack_range) {
                    tower->TakeDamage(player_->GetDamage());
                    attacked = true;

                    if (!tower->IsAlive()) {
                        field_->GetCell(tower_x, tower_y).SetEnemyTower(false);
                        player_->AddScore(40);
                    }
                }
            }

            if (attacked) {
                EndPlayerTurn();
            }
            return;
        }
        case PlayerAction::PAUSE_GAME:
            PauseGame();
            return;
        default:
            return;
    }

    if (player_->IsSlowed()) {
        player_->SetSlowed(false);
        EndPlayerTurn();
        return;
    }

    if (field_->IsValidPosition(new_x, new_y) && field_->IsPassable(new_x, new_y)) {
        Cell& new_cell = field_->GetCell(new_x, new_y);
        Cell& old_cell = field_->GetCell(old_x, old_y);

        if (new_cell.GetType() == CellType::SLOWING) {
            player_->SetSlowed(true);
        }

        if (!new_cell.HasEnemy() && !new_cell.HasEnemyBuilding() && !new_cell.HasEnemyTower()) {
            old_cell.SetPlayer(false);
            new_cell.SetPlayer(true);
            player_->SetPosition(new_x, new_y);

            NotifyEvent(GameEventFactory::PlayerMove(old_x, old_y, new_x, new_y));
        }

        EndPlayerTurn();
    }
}

void Game::EndPlayerTurn() {
    player_turn_ended_ = true;

    MoveEnemies();
    MoveAllies();
    UpdateEnemyBuildings();
    UpdateEnemyTowers();

    enemies_.erase(
        std::remove_if(enemies_.begin(), enemies_.end(),
            [](const std::unique_ptr<Enemy>& e) { return !e->IsAlive(); }),
        enemies_.end()
    );

    enemy_buildings_.erase(
        std::remove_if(enemy_buildings_.begin(), enemy_buildings_.end(),
            [](const std::unique_ptr<EnemyBuilding>& b) { return !b->IsAlive(); }),
        enemy_buildings_.end()
    );

    enemy_towers_.erase(
        std::remove_if(enemy_towers_.begin(), enemy_towers_.end(),
            [](const std::unique_ptr<EnemyTower>& t) { return !t->IsAlive(); }),
        enemy_towers_.end()
    );

    for (auto it = allies_.begin(); it != allies_.end(); ) {
        if (!(*it)->IsAlive()) {
            field_->GetCell((*it)->GetX(), (*it)->GetY()).SetAlly(false);
            it = allies_.erase(it);
        } else {
            ++it;
        }
    }

    if (!player_->IsAlive()) {
        state_ = GameState::GAME_OVER;
        NotifyEvent(GameEvent(EventType::PLAYER_DEATH, "Player has died"));
    }

    CheckLevelComplete();
    player_turn_ended_ = false;
}

void Game::MoveEnemies() {
    for (auto& enemy : enemies_) {
        if (!enemy->IsAlive()) continue;

        int enemy_x = enemy->GetX();
        int enemy_y = enemy->GetY();
        int player_x = player_->GetX();
        int player_y = player_->GetY();

        int dx = player_x - enemy_x;
        int dy = player_y - enemy_y;

        int move_x = 0, move_y = 0;
        if (std::abs(dx) > std::abs(dy)) {
            move_x = (dx > 0) ? 1 : -1;
        } else if (std::abs(dy) > 0) {
            move_y = (dy > 0) ? 1 : -1;
        }

        int new_x = enemy_x + move_x;
        int new_y = enemy_y + move_y;

        if (field_->IsValidPosition(new_x, new_y) && field_->IsPassable(new_x, new_y)) {
            Cell& new_cell = field_->GetCell(new_x, new_y);

            if (new_cell.HasTrap()) {
                int trap_damage = new_cell.GetTrapDamage();
                enemy->TakeDamage(trap_damage);
                new_cell.SetTrap(false, 0);
                NotifyEvent(GameEventFactory::TrapTriggered(new_x, new_y, trap_damage));
            }
        }

        if (new_x == player_x && new_y == player_y) {
            player_->TakeDamage(enemy->GetDamage());
            NotifyEvent(GameEventFactory::PlayerDamage(enemy->GetDamage(), player_->GetHealth()));
        } else if (field_->IsValidPosition(new_x, new_y) && field_->IsPassable(new_x, new_y)) {
            Cell& new_cell = field_->GetCell(new_x, new_y);
            Cell& old_cell = field_->GetCell(enemy_x, enemy_y);

            if (!new_cell.HasEnemy() && !new_cell.HasPlayer() && !new_cell.HasAlly()) {
                old_cell.SetEnemy(false);
                new_cell.SetEnemy(true);
                enemy->SetPosition(new_x, new_y);
            }
        }
    }
}

void Game::MoveAllies() {
    for (auto& ally : allies_) {
        if (!ally->IsAlive()) continue;

        int ally_x = ally->GetX();
        int ally_y = ally->GetY();

        Enemy* closest_enemy = nullptr;
        int min_distance = 999999;

        for (auto& enemy : enemies_) {
            if (!enemy->IsAlive()) continue;
            int dist = CalculateDistance(ally_x, ally_y, enemy->GetX(), enemy->GetY());
            if (dist < min_distance) {
                min_distance = dist;
                closest_enemy = enemy.get();
            }
        }

        if (closest_enemy) {
            int target_x = closest_enemy->GetX();
            int target_y = closest_enemy->GetY();

            int dx = target_x - ally_x;
            int dy = target_y - ally_y;

            int move_x = 0, move_y = 0;
            if (std::abs(dx) > std::abs(dy)) {
                move_x = (dx > 0) ? 1 : -1;
            } else if (std::abs(dy) > 0) {
                move_y = (dy > 0) ? 1 : -1;
            }

            int new_x = ally_x + move_x;
            int new_y = ally_y + move_y;

            if (new_x == target_x && new_y == target_y) {
                closest_enemy->TakeDamage(ally->GetDamage());
                if (!closest_enemy->IsAlive()) {
                    field_->GetCell(target_x, target_y).SetEnemy(false);
                    player_->AddScore(closest_enemy->GetScoreValue());
                    player_->AddExperience(15);
                    enemies_killed_++;
                }
            } else if (field_->IsValidPosition(new_x, new_y) && field_->IsPassable(new_x, new_y)) {
                Cell& new_cell = field_->GetCell(new_x, new_y);
                Cell& old_cell = field_->GetCell(ally_x, ally_y);

                if (!new_cell.HasEnemy() && !new_cell.HasPlayer() && !new_cell.HasAlly()) {
                    old_cell.SetAlly(false);
                    new_cell.SetAlly(true);
                    ally->SetPosition(new_x, new_y);
                }
            }
        }
    }
}

void Game::UpdateEnemyBuildings() {
    std::random_device rd;
    std::mt19937 gen(rd());

    for (auto& building : enemy_buildings_) {
        if (!building->IsAlive()) continue;

        if (building->ShouldSpawnEnemy()) {
            int bx = building->GetX();
            int by = building->GetY();

            std::vector<std::pair<int, int>> adjacent_cells;
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dx == 0 && dy == 0) continue;
                    int check_x = bx + dx;
                    int check_y = by + dy;
                    if (field_->IsValidPosition(check_x, check_y) &&
                        field_->IsPassable(check_x, check_y)) {
                        Cell& cell = field_->GetCell(check_x, check_y);
                        if (!cell.HasEnemy() && !cell.HasPlayer() && !cell.HasAlly()) {
                            adjacent_cells.push_back({check_x, check_y});
                        }
                    }
                }
            }

            if (!adjacent_cells.empty()) {
                std::uniform_int_distribution<> dist(0, adjacent_cells.size() - 1);
                auto pos = adjacent_cells[dist(gen)];

                int health = 30 + (current_level_ - 1) * 10;
                int damage = 10 + (current_level_ - 1) * 3;
                auto enemy = std::make_unique<Enemy>(pos.first, pos.second, health, damage);
                field_->GetCell(pos.first, pos.second).SetEnemy(true);

                NotifyEvent(GameEventFactory::EnemySpawn(pos.first, pos.second, health));
                enemies_.push_back(std::move(enemy));
            }
        }
    }
}

void Game::UpdateEnemyTowers() {
    for (auto& tower : enemy_towers_) {
        if (!tower->IsAlive()) continue;

        tower->UpdateCooldown();

        if (tower->CanAttack()) {
            int tower_x = tower->GetX();
            int tower_y = tower->GetY();
            int player_x = player_->GetX();
            int player_y = player_->GetY();

            int distance = CalculateDistance(tower_x, tower_y, player_x, player_y);

            if (distance <= tower->GetRange()) {
                player_->TakeDamage(tower->GetDamage());
                tower->Attack();
                NotifyEvent(GameEventFactory::PlayerDamage(tower->GetDamage(), player_->GetHealth()));
            }
        }
    }
}

void Game::CheckLevelComplete() {
    if (enemies_.empty() && enemy_buildings_.empty() && enemy_towers_.empty()) {
        state_ = GameState::LEVEL_COMPLETE;
        NotifyEvent(GameEventFactory::LevelComplete(current_level_, player_->GetScore()));
    }
}

void Game::NextLevel() {
    if (state_ != GameState::LEVEL_COMPLETE) {
        return;
    }

    current_level_++;
    state_ = GameState::UPGRADE_MENU;
}

void Game::UpgradePlayer(int choice) {
    if (state_ != GameState::UPGRADE_MENU) {
        return;
    }

    std::string upgrade_name;
    switch (choice) {
        case 0:
            player_->IncreaseMaxHealth(20);
            upgrade_name = "Max Health +20";
            break;
        case 1:
            player_->IncreaseDamage(5, 3);
            upgrade_name = "Damage +5/+3";
            break;
        case 2:
            if (player_hand_->GetSpellCount() < player_hand_->GetMaxSize()) {
                Spell* new_spell = CreateRandomSpell();
                player_hand_->AddSpell(new_spell);
                upgrade_name = "New Spell: " + new_spell->GetName();
            }
            break;
    }

    NotifyEvent(GameEventFactory::UpgradeSelected(upgrade_name));

    player_->LevelUp();
    player_->RestoreHealth();

    int spells_to_remove = player_hand_->GetSpellCount() / 2;
    player_hand_->RemoveRandomSpells(spells_to_remove);

    ClearLevel();

    int new_field_size = std::min(20, 15 + current_level_);
    field_ = std::make_unique<GameField>(new_field_size, new_field_size);

    int start_x = 2, start_y = 2;
    while (!field_->IsPassable(start_x, start_y)) {
        start_x++;
        if (start_x >= new_field_size - 2) {
            start_x = 2;
            start_y++;
        }
    }

    player_->SetPosition(start_x, start_y);
    field_->GetCell(start_x, start_y).SetPlayer(true);

    InitializeLevel();
    state_ = GameState::PLAYING;
}

void Game::InitializeLevel() {
    int base_enemies = 3 + current_level_;
    int base_buildings = std::min(2, current_level_ / 2);
    int base_towers = std::min(1, (current_level_ - 1) / 3);

    SpawnEnemies(base_enemies);
    SpawnEnemyBuildings(base_buildings);
    SpawnEnemyTowers(base_towers);

    NotifyEvent(GameEvent(EventType::LEVEL_START,
        "Level " + std::to_string(current_level_) + " started"));
}

void Game::SpawnEnemies(int count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> x_dist(1, field_->GetWidth() - 2);
    std::uniform_int_distribution<> y_dist(1, field_->GetHeight() - 2);

    for (int i = 0; i < count; ++i) {
        int x, y;
        int attempts = 0;
        do {
            x = x_dist(gen);
            y = y_dist(gen);
            attempts++;
        } while (attempts < 100 && (!field_->IsPassable(x, y) ||
                                    field_->GetCell(x, y).HasPlayer() ||
                                    field_->GetCell(x, y).HasEnemy() ||
                                    CalculateDistance(x, y, player_->GetX(), player_->GetY()) < 5));

        if (attempts < 100) {
            int health = 30 + (current_level_ - 1) * 10;
            int damage = 10 + (current_level_ - 1) * 3;
            auto enemy = std::make_unique<Enemy>(x, y, health, damage);
            field_->GetCell(x, y).SetEnemy(true);
            enemies_.push_back(std::move(enemy));
        }
    }
}

void Game::SpawnEnemyBuildings(int count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> x_dist(1, field_->GetWidth() - 2);
    std::uniform_int_distribution<> y_dist(1, field_->GetHeight() - 2);

    for (int i = 0; i < count; ++i) {
        int x, y;
        int attempts = 0;
        do {
            x = x_dist(gen);
            y = y_dist(gen);
            attempts++;
        } while (attempts < 100 && (!field_->IsPassable(x, y) ||
                                    field_->GetCell(x, y).HasPlayer() ||
                                    field_->GetCell(x, y).HasEnemy() ||
                                    field_->GetCell(x, y).HasEnemyBuilding() ||
                                    CalculateDistance(x, y, player_->GetX(), player_->GetY()) < 6));

        if (attempts < 100) {
            int health = 50 + (current_level_ - 1) * 15;
            auto building = std::make_unique<EnemyBuilding>(x, y, 3, health);
            field_->GetCell(x, y).SetEnemyBuilding(true);
            enemy_buildings_.push_back(std::move(building));
        }
    }
}

void Game::SpawnEnemyTowers(int count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> x_dist(1, field_->GetWidth() - 2);
    std::uniform_int_distribution<> y_dist(1, field_->GetHeight() - 2);

    for (int i = 0; i < count; ++i) {
        int x, y;
        int attempts = 0;
        do {
            x = x_dist(gen);
            y = y_dist(gen);
            attempts++;
        } while (attempts < 100 && (!field_->IsPassable(x, y) ||
                                    field_->GetCell(x, y).HasPlayer() ||
                                    field_->GetCell(x, y).HasEnemy() ||
                                    field_->GetCell(x, y).HasEnemyBuilding() ||
                                    field_->GetCell(x, y).HasEnemyTower() ||
                                    CalculateDistance(x, y, player_->GetX(), player_->GetY()) < 7));

        if (attempts < 100) {
            int health = 60 + (current_level_ - 1) * 20;
            int damage = 15 + (current_level_ - 1) * 4;
            auto tower = std::make_unique<EnemyTower>(x, y, health, damage);
            field_->GetCell(x, y).SetEnemyTower(true);
            enemy_towers_.push_back(std::move(tower));
        }
    }
}

void Game::ClearLevel() {
    if (field_) {
        for (int y = 0; y < field_->GetHeight(); ++y) {
            for (int x = 0; x < field_->GetWidth(); ++x) {
                Cell& cell = field_->GetCell(x, y);
                cell.SetPlayer(false);
                cell.SetEnemy(false);
                cell.SetEnemyBuilding(false);
                cell.SetEnemyTower(false);
                cell.SetAlly(false);
                cell.SetTrap(false, 0);
            }
        }
    }

    enemies_.clear();
    enemy_buildings_.clear();
    enemy_towers_.clear();
    allies_.clear();
}

int Game::CalculateDistance(int x1, int y1, int x2, int y2) const {
    return std::max(std::abs(x2 - x1), std::abs(y2 - y1));
}

void Game::ApplyEnhancementToSpell(Spell* spell) {
    int stacks = player_hand_->GetEnhancementStacks();
    for (int i = 0; i < stacks; ++i) {
        spell->Enhance();
    }
}

Spell* Game::CreateRandomSpell() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> spell_dist(0, 4);
    int random_spell = spell_dist(gen);

    switch (random_spell) {
        case 0: return new DirectDamageSpell();
        case 1: return new AreaDamageSpell();
        case 2: return new TrapSpell();
        case 3: return new SummonSpell();
        case 4: return new EnhancementSpell();
        default: return new DirectDamageSpell();
    }
}


void Game::SaveGame(const std::string& filename) {
    std::filesystem::create_directories("saves");

    std::ofstream file("saves/" + filename);
    if (!file.is_open()) {
        throw FileOpenException(filename);
    }

    try {
        file << current_level_ << "\n";
        file << enemies_killed_ << "\n";
        file << player_->GetHealth() << " " << player_->GetMaxHealth() << "\n";
        file << player_->GetMeleeDamage() << " " << player_->GetRangedDamage() << "\n";
        file << player_->GetScore() << "\n";
        file << player_->GetX() << " " << player_->GetY() << "\n";
        file << static_cast<int>(player_->GetCombatMode()) << "\n";
        file << player_->GetExperiencePoints() << "\n";
        file << player_->GetLevel() << "\n";

        if (file.fail()) {
            throw FileWriteException(filename, "player data");
        }

        if (field_) {
            field_->Serialize(file);
        } else {
            file << "10 10\n";
        }

        if (file.fail()) {
            throw FileWriteException(filename, "field data");
        }

        file << player_hand_->GetSpellCount() << "\n";
        for (int i = 0; i < player_hand_->GetSpellCount(); ++i) {
            Spell* spell = player_hand_->GetSpell(i);
            file << static_cast<int>(spell->GetType()) << " " << spell->GetLevel() << "\n";
        }

        file << enemies_.size() << "\n";
        for (const auto& enemy : enemies_) {
            file << enemy->GetX() << " " << enemy->GetY() << " "
                 << enemy->GetHealth() << " " << enemy->GetMaxHealth() << " "
                 << enemy->GetDamage() << "\n";
        }

        file << enemy_buildings_.size() << "\n";
        for (const auto& building : enemy_buildings_) {
            file << building->GetX() << " " << building->GetY() << " "
                 << building->GetHealth() << " " << building->GetMaxHealth() << " "
                 << building->GetSpawnInterval() << " " << building->GetTurnsSinceLastSpawn() << "\n";
        }

        file << enemy_towers_.size() << "\n";
        for (const auto& tower : enemy_towers_) {
            file << tower->GetX() << " " << tower->GetY() << " "
                 << tower->GetHealth() << " " << tower->GetMaxHealth() << " "
                 << tower->GetDamage() << " " << tower->GetRange() << " "
                 << tower->GetCooldown() << " " << tower->GetCurrentCooldown() << "\n";
        }

        file << allies_.size() << "\n";
        for (const auto& ally : allies_) {
            file << ally->GetX() << " " << ally->GetY() << " "
                 << ally->GetHealth() << " " << ally->GetMaxHealth() << " "
                 << ally->GetDamage() << "\n";
        }

        if (file.fail()) {
            throw FileWriteException(filename, "entity data");
        }

        NotifyEvent(GameEventFactory::SaveGame(filename));
    } catch (const std::exception& e) {
        throw SaveGameException(std::string("Failed to save game: ") + e.what());
    }
}

void Game::LoadGame(const std::string& filename) {
    std::ifstream file("saves/" + filename);
    if (!file.is_open()) {
        throw FileOpenException(filename);
    }

    try {
        int level, killed;
        file >> level >> killed;
        if (file.fail()) {
            throw CorruptedSaveException("header", "failed to read level/killed count");
        }

        int health, max_health, melee_damage, ranged_damage;
        int score, x, y, combat_mode, exp, player_level;

        file >> health >> max_health >> melee_damage >> ranged_damage;
        file >> score >> x >> y >> combat_mode >> exp >> player_level;

        if (file.fail()) {
            throw CorruptedSaveException("player", "failed to read player data");
        }

        ClearLevel();

        field_ = std::make_unique<GameField>(10, 10);
        field_->Deserialize(file);

        player_ = std::make_unique<Player>(x, y, max_health, melee_damage, ranged_damage);
        player_->TakeDamage(max_health - health);
        player_->AddScore(score);
        player_->AddExperience(exp);
        for (int i = 1; i < player_level; ++i) {
            player_->LevelUp();
        }
        if (combat_mode == 1) {
            player_->SwitchCombatMode();
        }

        field_->GetCell(x, y).SetPlayer(true);

        player_hand_ = std::make_unique<PlayerHand>(5);
        int spell_count;
        file >> spell_count;

        if (file.fail() || spell_count < 0) {
            throw CorruptedSaveException("spells", "invalid spell count");
        }

        for (int i = 0; i < spell_count; ++i) {
            int spell_type, spell_level;
            file >> spell_type >> spell_level;

            if (file.fail()) {
                throw CorruptedSaveException("spells", "failed to read spell data");
            }

            Spell* spell = nullptr;
            switch (static_cast<SpellType>(spell_type)) {
                case SpellType::DIRECT_DAMAGE:
                    spell = new DirectDamageSpell();
                    break;
                case SpellType::AREA_DAMAGE:
                    spell = new AreaDamageSpell();
                    break;
                case SpellType::TRAP:
                    spell = new TrapSpell();
                    break;
                case SpellType::SUMMON:
                    spell = new SummonSpell();
                    break;
                case SpellType::ENHANCEMENT:
                    spell = new EnhancementSpell();
                    break;
                default:
                    throw CorruptedSaveException("spells", "unknown spell type");
            }

            for (int j = 1; j < spell_level; ++j) {
                spell->Enhance();
            }
            player_hand_->AddSpell(spell);
        }

        size_t enemy_count;
        file >> enemy_count;
        for (size_t i = 0; i < enemy_count; ++i) {
            int ex, ey, ehealth, emax_health, edamage;
            file >> ex >> ey >> ehealth >> emax_health >> edamage;

            if (file.fail()) {
                throw CorruptedSaveException("enemies", "failed to read enemy data");
            }

            auto enemy = std::make_unique<Enemy>(ex, ey, emax_health, edamage);
            enemy->TakeDamage(emax_health - ehealth);
            enemies_.push_back(std::move(enemy));
        }

        size_t building_count;
        file >> building_count;
        for (size_t i = 0; i < building_count; ++i) {
            int bx, by, bhealth, bmax_health, bspawn_interval, bturns_since_spawn;
            file >> bx >> by >> bhealth >> bmax_health >> bspawn_interval >> bturns_since_spawn;

            if (file.fail()) {
                throw CorruptedSaveException("buildings", "failed to read building data");
            }

            auto building = std::make_unique<EnemyBuilding>(bx, by, bspawn_interval, bmax_health);
            building->TakeDamage(bmax_health - bhealth);
            building->SetTurnsSinceLastSpawn(bturns_since_spawn);
            enemy_buildings_.push_back(std::move(building));
        }

        size_t tower_count;
        file >> tower_count;
        for (size_t i = 0; i < tower_count; ++i) {
            int tx, ty, thealth, tmax_health, tdamage, trange, tcooldown, tcurrent_cooldown;
            file >> tx >> ty >> thealth >> tmax_health >> tdamage >> trange >> tcooldown >> tcurrent_cooldown;

            if (file.fail()) {
                throw CorruptedSaveException("towers", "failed to read tower data");
            }

            auto tower = std::make_unique<EnemyTower>(tx, ty, tmax_health, tdamage, trange, tcooldown);
            tower->TakeDamage(tmax_health - thealth);
            tower->SetCurrentCooldown(tcurrent_cooldown);
            enemy_towers_.push_back(std::move(tower));
        }

        size_t ally_count;
        file >> ally_count;
        for (size_t i = 0; i < ally_count; ++i) {
            int ax, ay, ahealth, amax_health, adamage;
            file >> ax >> ay >> ahealth >> amax_health >> adamage;

            if (file.fail()) {
                throw CorruptedSaveException("allies", "failed to read ally data");
            }

            auto ally = std::make_unique<Ally>(ax, ay, amax_health, adamage);
            ally->TakeDamage(amax_health - ahealth);
            allies_.push_back(std::move(ally));
        }

        current_level_ = level;
        enemies_killed_ = killed;
        state_ = GameState::PLAYING;

        NotifyEvent(GameEventFactory::LoadGame(filename));
    } catch (const CorruptedSaveException&) {
        throw;
    } catch (const std::exception& e) {
        throw LoadGameException(std::string("Failed to load game: ") + e.what());
    }
}

GameState Game::GetState() const {
    return state_;
}

const GameField* Game::GetField() const {
    return field_.get();
}

const Player* Game::GetPlayer() const {
    return player_.get();
}

const PlayerHand* Game::GetPlayerHand() const {
    return player_hand_.get();
}

const std::vector<std::unique_ptr<Enemy>>& Game::GetEnemies() const {
    return enemies_;
}

const std::vector<std::unique_ptr<EnemyBuilding>>& Game::GetEnemyBuildings() const {
    return enemy_buildings_;
}

const std::vector<std::unique_ptr<EnemyTower>>& Game::GetEnemyTowers() const {
    return enemy_towers_;
}

const std::vector<std::unique_ptr<Ally>>& Game::GetAllies() const {
    return allies_;
}

int Game::GetCurrentLevel() const {
    return current_level_;
}

int Game::GetEnemiesKilled() const {
    return enemies_killed_;
}

void Game::AddEventObserver(EventObserver* observer) {
    event_observers_.push_back(observer);
}

void Game::RemoveEventObserver(EventObserver* observer) {
    event_observers_.erase(
        std::remove(event_observers_.begin(), event_observers_.end(), observer),
        event_observers_.end()
    );
}

void Game::NotifyEvent(const GameEvent& event) {
    for (auto* observer : event_observers_) {
        observer->OnEvent(event);
    }
}
