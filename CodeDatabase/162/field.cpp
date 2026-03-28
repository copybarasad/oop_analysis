#include "field.h"
#include "ally.h"
#include "input_handler.h"
#include "renderer.h"
#include <string> 
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <limits>

Field::Field(int width, int height, int turns_to_spawn_enemy, int obstacle_count, int slow_count, int building_count, std::shared_ptr<Player> player_param) 
    : width_(width), height_(height), turns_to_spawn_enemy_(turns_to_spawn_enemy), player_(player_param), under_player_type_(CellType::kEmpty), player_slowed_(false), turn_count_(0) {
    grid_.resize(height_, std::vector<Cell>(width_, Cell(CellType::kEmpty)));
    player_x_ = width_ / 2;
    player_y_ = height_ / 2;
    if (!player_param) {
        InitializeObstaclesAndSlowCells(obstacle_count, slow_count, building_count);
        grid_[player_y_][player_x_].SetType(CellType::kPlayer);
        SpawnInitialEnemies(3);
    }
}

Field::Field(const Field& other)
    : width_(other.width_), height_(other.height_),
      turns_to_spawn_enemy_(other.turns_to_spawn_enemy_),  
      grid_(other.grid_),
      player_(other.player_ ? std::make_unique<Player>(*other.player_) : nullptr),
      player_x_(other.player_x_), player_y_(other.player_y_),
      under_player_type_(other.under_player_type_),
      player_slowed_(other.player_slowed_),
      enemy_positions_(other.enemy_positions_),
      under_enemy_type_(other.under_enemy_type_),
      enemy_slowed_(other.enemy_slowed_),
      building_positions_(other.building_positions_),
      trap_positions_(other.trap_positions_),
      under_trap_type_(other.under_trap_type_),
      tower_positions_(other.tower_positions_),
      under_tower_type_(other.under_tower_type_),
      ally_positions_(other.ally_positions_),
      under_ally_type_(other.under_ally_type_),
      turn_count_(other.turn_count_) {
    enemies_.reserve(other.enemies_.size());
    for (const auto& e : other.enemies_) {
        enemies_.emplace_back(std::make_unique<Enemy>(*e));
    }
    buildings_.reserve(other.buildings_.size());
    for (const auto& b : other.buildings_) {
        buildings_.emplace_back(std::make_unique<Building>(*b));
    }
    traps_.reserve(other.traps_.size());
    for (const auto& t : other.traps_) {
        traps_.emplace_back(std::make_unique<Trap>(t->GetX(), t->GetY(), t->GetDamage()));
    }
    towers_.reserve(other.towers_.size());
    for (const auto& tw : other.towers_) {
        towers_.emplace_back(std::make_unique<EnemyTower>(tw->GetX(), tw->GetY()));
    }
    allies_.reserve(other.allies_.size());
    for (const auto& a : other.allies_) {
        allies_.emplace_back(std::make_unique<Ally>(a->GetX(), a->GetY(), a->GetHealth(), a->GetDamage()));
    }
}


Field::Field(Field&& other) noexcept
    : width_(std::move(other.width_)), height_(std::move(other.height_)),
      turns_to_spawn_enemy_(std::move(other.turns_to_spawn_enemy_)),  
      grid_(std::move(other.grid_)),
      player_(std::move(other.player_)),
      player_x_(std::move(other.player_x_)), player_y_(std::move(other.player_y_)),
      under_player_type_(std::move(other.under_player_type_)),
      player_slowed_(std::move(other.player_slowed_)),
      enemies_(std::move(other.enemies_)),
      enemy_positions_(std::move(other.enemy_positions_)),
      under_enemy_type_(std::move(other.under_enemy_type_)),
      enemy_slowed_(std::move(other.enemy_slowed_)),
      buildings_(std::move(other.buildings_)),
      building_positions_(std::move(other.building_positions_)),
      traps_(std::move(other.traps_)),
      trap_positions_(std::move(other.trap_positions_)),
      under_trap_type_(std::move(other.under_trap_type_)),
      towers_(std::move(other.towers_)),
      tower_positions_(std::move(other.tower_positions_)),
      under_tower_type_(std::move(other.under_tower_type_)),
      allies_(std::move(other.allies_)),
      ally_positions_(std::move(other.ally_positions_)),
      under_ally_type_(std::move(other.under_ally_type_)),
      turn_count_(std::move(other.turn_count_)) {}

Field& Field::operator=(const Field& other) {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        turns_to_spawn_enemy_ = other.turns_to_spawn_enemy_;  
        grid_ = other.grid_;
        player_ = other.player_ ? std::make_unique<Player>(*other.player_) : nullptr;
        player_x_ = other.player_x_;
        player_y_ = other.player_y_;
        under_player_type_ = other.under_player_type_;
        player_slowed_ = other.player_slowed_;
        enemy_positions_ = other.enemy_positions_;
        under_enemy_type_ = other.under_enemy_type_;
        enemy_slowed_ = other.enemy_slowed_;
        building_positions_ = other.building_positions_;
        turn_count_ = other.turn_count_;
        trap_positions_ = other.trap_positions_;
        under_trap_type_ = other.under_trap_type_;
        tower_positions_ = other.tower_positions_;
        under_tower_type_ = other.under_tower_type_;
        ally_positions_ = other.ally_positions_;
        under_ally_type_ = other.under_ally_type_;
        enemies_.clear();
        enemies_.reserve(other.enemies_.size());
        for (const auto& e : other.enemies_) {
            enemies_.emplace_back(std::make_unique<Enemy>(*e));
        }
        buildings_.clear();
        buildings_.reserve(other.buildings_.size());
        for (const auto& b : other.buildings_) {
            buildings_.emplace_back(std::make_unique<Building>(*b));
        }
        traps_.clear();
        traps_.reserve(other.traps_.size());
        for (const auto& t : other.traps_) {
            traps_.emplace_back(std::make_unique<Trap>(t->GetX(), t->GetY(), t->GetDamage()));
        }
        towers_.clear();
        towers_.reserve(other.towers_.size());
        for (const auto& tw : other.towers_) {
            towers_.emplace_back(std::make_unique<EnemyTower>(tw->GetX(), tw->GetY()));
        }
        allies_.clear();
        allies_.reserve(other.allies_.size());
        for (const auto& a : other.allies_) {
            allies_.emplace_back(std::make_unique<Ally>(a->GetX(), a->GetY(), a->GetHealth(), a->GetDamage()));
        }
    }
    return *this;
}

Field& Field::operator=(Field&& other) noexcept {
    if (this != &other) {
        width_ = std::move(other.width_);
        height_ = std::move(other.height_);
        turns_to_spawn_enemy_ = std::move(other.turns_to_spawn_enemy_);
        grid_ = std::move(other.grid_);
        player_ = std::move(other.player_);
        player_x_ = std::move(other.player_x_);
        player_y_ = std::move(other.player_y_);
        under_player_type_ = other.under_player_type_;
        player_slowed_ = other.player_slowed_;
        enemies_ = std::move(other.enemies_);
        enemy_positions_ = std::move(other.enemy_positions_);
        under_enemy_type_ = std::move(other.under_enemy_type_);
        enemy_slowed_ = std::move(other.enemy_slowed_);
        buildings_ = std::move(other.buildings_);
        building_positions_ = std::move(other.building_positions_);
        trap_positions_ = std::move(other.trap_positions_);
        under_trap_type_ = std::move(other.under_trap_type_);
        tower_positions_ = std::move(other.tower_positions_);
        under_tower_type_ = std::move(other.under_tower_type_);
        ally_positions_ = std::move(other.ally_positions_);
        under_ally_type_ = std::move(other.under_ally_type_);
        traps_ = std::move(other.traps_);
        towers_ = std::move(other.towers_);
        allies_ = std::move(other.allies_);
        turn_count_ = std::move(other.turn_count_);
    }
    return *this;
}

Field::~Field() = default;

void Field::AddTower(int x, int y) {
    if (IsValidPosition(x, y) && grid_[y][x].GetType() == CellType::kEmpty) {
        towers_.push_back(std::make_unique<EnemyTower>(x, y));
        under_tower_type_.push_back(grid_[y][x].GetType());
        tower_positions_.push_back({x, y});
        grid_[y][x].SetType(CellType::kTower);
    }
}

void Field::InitializeObstaclesAndSlowCells(int obstacle_count, int slow_count, int building_count) {
    srand(time(0));
   
    for (int i = 0; i < obstacle_count; ++i) {
        int x = rand() % width_;
        int y = rand() % height_;
        if (grid_[y][x].GetType() == CellType::kEmpty && !(x == player_x_ && y == player_y_)) {
            grid_[y][x].SetType(CellType::kObstacle);
        }
    }

    for (int i = 0; i < slow_count; ++i) {
        int x = rand() % width_;
        int y = rand() % height_;
        if (grid_[y][x].GetType() == CellType::kEmpty && !(x == player_x_ && y == player_y_)) {
            grid_[y][x].SetType(CellType::kSlow);
        }
    }

    for (int i = 0; i < building_count; ++i) {
        bool placed = false;
        int attempts = 0;
        while (!placed && attempts < 100) {
            int x = rand() % width_;
            int y = rand() % height_;
            if (grid_[y][x].GetType() == CellType::kEmpty && !(x == player_x_ && y == player_y_)) {
                grid_[y][x].SetType(CellType::kBuilding);
                buildings_.emplace_back(std::make_unique<Building>(50, 10, x, y));
                building_positions_.emplace_back(x, y);
                placed = true;
            }
            attempts++;
        }
    }
    SpawnInitialEnemies(3);
}

void Field::SpawnInitialEnemies(int enemy_count) {
    for (int i = 0; i < enemy_count; ++i) {
        bool placed = false;
        int attempts = 0;
        while (!placed && attempts < 100) {
            int x = rand() % width_;
            int y = rand() % height_;
            if (grid_[y][x].GetType() == CellType::kEmpty && !(x == player_x_ && y == player_y_)) {
                CellType original_type = grid_[y][x].GetType();  
                auto new_enemy = std::make_unique<Enemy>(50, 10, x, y);  
                enemies_.emplace_back(std::move(new_enemy));
                enemy_positions_.emplace_back(x, y);
                grid_[y][x].SetType(CellType::kEnemy);
                under_enemy_type_.push_back(original_type);
                enemy_slowed_.push_back(false);  
                placed = true;
            }
            attempts++;
        }
    }
}

void Field::ProcessBuildings() {
    turn_count_++;
    if (turn_count_ % turns_to_spawn_enemy_ == 0) {
        for (size_t i = 0; i < buildings_.size(); ++i) {
            Enemy* new_enemy = buildings_[i]->SpawnEnemy();
            if (new_enemy) {
                int bx = building_positions_[i].first;
                int by = building_positions_[i].second;
                std::vector<std::pair<int, int>> directions = {
                    {0, 1}, {1, 0}, {0, -1}, {-1, 0},
                    {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
                };
                bool placed = false;
                for (auto& dir : directions) {
                    int ex = bx + dir.first;
                    int ey = by + dir.second;
                    if (ex >= 0 && ex < width_ && ey >= 0 && ey < height_ && grid_[ey][ex].GetType() == CellType::kEmpty) {
                        CellType original_type = grid_[ey][ex].GetType();  
                        new_enemy->SetPosition(ex, ey);
                        enemies_.emplace_back(new_enemy);
                        enemy_positions_.emplace_back(ex, ey);
                        grid_[ey][ex].SetType(CellType::kEnemy);
                        under_enemy_type_.push_back(original_type);  
                        enemy_slowed_.push_back(false);
                        placed = true;
                        break;
                    }
                }
                if (!placed) {
                    delete new_enemy;
                }
            }
        }
    }
    if (turn_count_ % 10 == 0) {
        int x = rand() % width_;
        int y = rand() % height_;
        if (IsEmpty(x, y)) {
            AddTower(x, y);
        }
    }
}

bool Field::IsCellPassable(int x, int y) const {
    if (!IsValidPosition(x, y)) return false;
    CellType type = grid_[y][x].GetType();
    return type != CellType::kObstacle;
}

bool Field::IsEmpty(int x, int y) const {
    if (!IsValidPosition(x, y)) return false;
    CellType type = grid_[y][x].GetType();
    return type == CellType::kEmpty;
}

bool Field::MovePlayer(int dx, int dy) {
    int nx = player_x_ + dx;
    int ny = player_y_ + dy;
    if (nx >= 0 && nx < width_ && ny >= 0 && ny < height_ && 
        grid_[ny][nx].GetType() != CellType::kObstacle && 
        grid_[ny][nx].GetType() != CellType::kEnemy && 
        grid_[ny][nx].GetType() != CellType::kBuilding &&
        grid_[ny][nx].GetType() != CellType::kTower &&
        grid_[ny][nx].GetType() != CellType::kAlly) {
        grid_[player_y_][player_x_].SetType(under_player_type_);
       
        under_player_type_ = grid_[ny][nx].GetType();
       
        if (under_player_type_ == CellType::kSlow) {
            player_slowed_ = true;
        }
       
        player_x_ = nx;
        player_y_ = ny;
        grid_[player_y_][player_x_].SetType(CellType::kPlayer);
        UpdateTraps();
        return true;
    } else {
        std::cout << "Cannot move there\n";
        return false;
    }
}

void Field::MoveEnemies() {
    for (auto& enemy : enemies_) {
        if (!enemy->IsAlive()) continue;
        
        int x = enemy->GetX();
        int y = enemy->GetY();
        
        int target_x = player_x_, target_y = player_y_;
        int min_dist = std::abs(x - player_x_) + std::abs(y - player_y_);
        bool target_is_ally = false;
        
        for (auto& ally : allies_) {
            if (!ally->IsAlive()) continue;
            int ax = ally->GetX(), ay = ally->GetY();
            int dist = std::abs(x - ax) + std::abs(y - ay);
            if (dist < min_dist) {
                min_dist = dist;
                target_x = ax;
                target_y = ay;
                target_is_ally = true;
            }
        }
        
        int dx = (target_x > x) ? 1 : (target_x < x) ? -1 : 0;
        int dy = (target_y > y) ? 1 : (target_y < y) ? -1 : 0;
        int nx = x + dx;
        int ny = y + dy;
        
        if (nx >= 0 && nx < width_ && ny >= 0 && ny < height_ &&
            grid_[ny][nx].GetType() != CellType::kObstacle &&
            grid_[ny][nx].GetType() != CellType::kBuilding &&
            grid_[ny][nx].GetType() != CellType::kTower &&
            grid_[ny][nx].GetType() != CellType::kEnemy) {
            
            if (grid_[ny][nx].GetType() == CellType::kPlayer) {
                player_->TakeDamage(enemy->GetDamage());
                std::cout << "Enemy at (" << x << ", " << y << ") attacked player for " << enemy->GetDamage() << " damage\n";
                if (!player_->IsAlive()) {
                    std::cout << "Player is dead!\n";
                    return;
                }
            } else if (grid_[ny][nx].GetType() == CellType::kAlly && target_is_ally) {
                for (auto& ally : allies_) {
                    if (ally->GetX() == nx && ally->GetY() == ny && ally->IsAlive()) {
                        ally->TakeDamage(enemy->GetDamage());
                        std::cout << "Enemy at (" << x << ", " << y << ") attacked ally at (" << nx << ", " << ny << ") for " << enemy->GetDamage() << " damage\n";
                        if (!ally->IsAlive()) {
                            RemoveAlly(ally.get());
                        }
                        break;
                    }
                }
            } else {
                grid_[y][x].SetType(under_enemy_type_[&enemy - &enemies_[0]]);
                under_enemy_type_[&enemy - &enemies_[0]] = grid_[ny][nx].GetType();
                enemy->SetPosition(nx, ny);
                if (nx != x || ny != y) { 
                    size_t idx = &enemy - &enemies_[0]; 
                    enemy_positions_[idx] = {nx, ny};   
                }
                grid_[ny][nx].SetType(CellType::kEnemy);
            }
        }
    }
}

EnemyTower* Field::GetTowerAt(int x, int y) const {
    for (const auto& tw : towers_) {
        if (tw->GetX() == x && tw->GetY() == y) return tw.get();
    }
    return nullptr;
}

void Field::RemoveTower(EnemyTower* tower) {
    auto it = std::find_if(towers_.begin(), towers_.end(), [tower](const std::unique_ptr<EnemyTower>& t) { return t.get() == tower; });
    if (it != towers_.end()) {
        size_t i = it - towers_.begin();
        int tx = tower->GetX(), ty = tower->GetY();
        grid_[ty][tx].SetType(under_tower_type_[i]);
        towers_.erase(it);
        tower_positions_.erase(tower_positions_.begin() + i);
        under_tower_type_.erase(under_tower_type_.begin() + i);
    }
}

void Field::RemoveAlly(Ally* ally) {
    auto it = std::find_if(allies_.begin(), allies_.end(), [ally](const std::unique_ptr<Ally>& a) { return a.get() == ally; });
    if (it != allies_.end()) {
        size_t i = it - allies_.begin();
        int ax = ally_positions_[i].first;
        int ay = ally_positions_[i].second;
        grid_[ay][ax].SetType(under_ally_type_[i]);
        allies_.erase(it);
        ally_positions_.erase(ally_positions_.begin() + i);
        under_ally_type_.erase(under_ally_type_.begin() + i);
    }
}

bool Field::AttackEnemiesAt(int x, int y) {
    int distance = std::abs(x - player_x_) + std::abs(y - player_y_);
    if (!player_->CanAttack(distance)) {
        std::cout << "Target too far! Distance: " << distance << "\n";
        return false;
    }

    if (!IsValidPosition(x, y)) {
        std::cout << "Invalid coordinates\n";
        return false;
    }

    CellType cell_type = grid_[y][x].GetType();
    int damage = player_->GetDamage();

    if (cell_type == CellType::kEnemy) {
        for (size_t i = 0; i < enemies_.size(); ++i) {
            if (enemy_positions_[i].first == x && enemy_positions_[i].second == y) {
                enemies_[i]->TakeDamage(damage);
                std::cout << "Attack dealt " << damage << " damage. Enemy health: " << enemies_[i]->GetHealth() << "\n";
                if (!enemies_[i]->IsAlive()) {
                    player_->AddPoints(10);
                    player_->KillEnemy();
                    grid_[y][x].SetType(under_enemy_type_[i]);
                    enemies_.erase(enemies_.begin() + i);
                    enemy_positions_.erase(enemy_positions_.begin() + i);
                    under_enemy_type_.erase(under_enemy_type_.begin() + i);
                    enemy_slowed_.erase(enemy_slowed_.begin() + i);
                    std::cout << "Enemy killed! Points: " << player_->GetPoints() << "\n";
                }
                return true;
            }
        }
    } else if (cell_type == CellType::kBuilding) {
        for (size_t i = 0; i < buildings_.size(); ++i) {
            if (building_positions_[i].first == x && building_positions_[i].second == y) {
                buildings_[i]->TakeDamage(damage);
                std::cout << "Attack dealt " << damage << " damage. Building health: " << buildings_[i]->GetHealth() << "\n";
                if (!buildings_[i]->IsAlive()) {
                    grid_[y][x].SetType(CellType::kEmpty);
                    buildings_.erase(buildings_.begin() + i);
                    building_positions_.erase(building_positions_.begin() + i);
                    std::cout << "Building destroyed!\n";
                }
                return true;
            }
        }
    } else if (cell_type == CellType::kTower) {
        for (const auto& tw : towers_) {
            if (tw->GetX() == x && tw->GetY() == y) {
                tw->TakeDamage(damage);
                std::cout << "Attack dealt " << damage << " damage. Tower health: " << tw->GetHealth() << "\n";
                if (tw->IsDestroyed()) {
                    RemoveTower(tw.get());
                    grid_[y][x].SetType(CellType::kEmpty);
                    std::cout << "Tower destroyed!\n";
                }
                return true;
            }
        }
    } else if (cell_type == CellType::kAlly) {
        std::cout << "Cannot attack ally!\n";
        return false;
    }

    std::cout << "No valid target in cell (" << x << ", " << y << ") or out of range\n";
    return false;
}


void Field::Print() const {
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            bool is_trap = false;
            for (const auto& t : traps_) {
                if (t->GetX() == x && t->GetY() == y) {
                    is_trap = true;
                    break;
                }
            }
            bool is_tower = false;
            for (const auto& tw : towers_) {
                if (tw->GetX() == x && tw->GetY() == y) {
                    is_tower = true;
                    break;
                }
            }
            bool is_ally = false;
            for (const auto& a : allies_) {
                if (a->GetX() == x && a->GetY() == y) {
                    is_ally = true;
                    break;
                }
            }
            if (is_trap) {
                std::cout << 'T';
            } else if (is_tower) {
                std::cout << 'W'; 
            } else if (is_ally) {
                std::cout << 'A';
            } else if (std::find(building_positions_.begin(), building_positions_.end(), std::make_pair(x, y)) != building_positions_.end()) {
                std::cout << 'B';
            } else {
                switch (grid_[y][x].GetType()) {
                    case CellType::kEmpty: std::cout << '.'; break;
                    case CellType::kObstacle: std::cout << '#'; break;
                    case CellType::kSlow: std::cout << '~'; break;
                    case CellType::kPlayer: std::cout << 'P'; break;
                    case CellType::kEnemy: std::cout << 'E'; break;
                    case CellType::kBuilding: std::cout << 'B'; break;
                    case CellType::kTrap: std::cout << 'T'; break;
                    case CellType::kTower: std::cout << 'W'; break;
                    case CellType::kAlly: std::cout << 'A'; break;
                }
            }
        }
        std::cout << '\n';
    }
    std::cout << "Player: health " << player_->GetHealth() << ", points " << player_->GetPoints() << ", fight mode: " << player_->GetFightModeName() << "\n";
}

void Field::RunGame(bool& save_requested, bool& load_requested, bool& quit_requested) {
    InputHandler inputHandler;
    Renderer renderer;
    std::string command;
    
    while (IsPlayerAlive()) {
        renderer.RenderGameState(*this);
        
        bool was_slowed = IsPlayerSlowed();
        if (was_slowed) {
            renderer.RenderMessage("You are slowed! Move commands ignored this turn.");
            ResetPlayerSlowed();
        }
        
        std::cout << "Enter command: ";
        std::getline(std::cin, command);
        
        auto cmd = inputHandler.ParseInput(command);
        
        if (cmd.action == "unknown" || cmd.action.empty()) {
            renderer.RenderMessage("Unknown command. Type 'help' for available commands.");
            continue;
        }
        
        bool turn_used = false;
        bool command_processed = false;
        
        if (cmd.action == "cast") {
            if (cmd.HasArgs(3)) {
                int index = cmd.GetIntArg(0);
                int x = cmd.GetIntArg(1);
                int y = cmd.GetIntArg(2);
                if (player_->GetHand().UseSpell(index, *player_, *this, x, y)) {
                    renderer.RenderMessage("Spell cast successfully!");
                    turn_used = true;
                } else {
                    renderer.RenderMessage("Failed to cast spell.");
                }
            } else {
                renderer.RenderMessage("Invalid cast command. Use: cast <index> <x> <y>");
            }
            command_processed = true;
        } 
        else if (cmd.action == "w" || cmd.action == "a" || cmd.action == "s" || cmd.action == "d") {
            if (!was_slowed && !turn_used) {
                int dx = 0, dy = 0;
                if (cmd.action == "w") dy = -1;
                else if (cmd.action == "a") dx = -1;
                else if (cmd.action == "s") dy = 1;
                else if (cmd.action == "d") dx = 1;
                
                if (MovePlayer(dx, dy)) {
                    turn_used = true;
                }
            }
            command_processed = true;
        }
        else if (cmd.action == "attack") {
            if (!turn_used && cmd.HasArgs(2)) {
                int x = cmd.GetIntArg(0);
                int y = cmd.GetIntArg(1);
                if (AttackEnemiesAt(x, y)) {
                    turn_used = true;
                }
            } else {
                renderer.RenderMessage("Invalid attack command. Use: attack <x> <y>");
            }
            command_processed = true;
        }
        else if (cmd.action == "switch_mode") {
            PlayerSwitchMode();
            renderer.RenderMessage("Switched to " + player_->GetFightModeName() + " mode.");
            command_processed = true;
        }
        else if (cmd.action == "buy_spell") {
            player_->BuySpell();
            command_processed = true;
        }
        else if (cmd.action == "info") {
            std::cout << "\n=== DETAILED INFO ===\n";
            std::cout << "Player position: (" << player_x_ << ", " << player_y_ << ")\n";
            
            std::pair<int, int> closest_enemy = {-1, -1};
            int min_dist_enemy = INT_MAX;
            for (auto& pos : enemy_positions_) {
                int dist = std::abs(pos.first - player_x_) + std::abs(pos.second - player_y_);
                if (dist < min_dist_enemy) {
                    min_dist_enemy = dist;
                    closest_enemy = pos;
                }
            }
            if (closest_enemy.first != -1) {
                std::cout << "Closest enemy: (" << closest_enemy.first << ", " 
                          << closest_enemy.second << ") distance: " << min_dist_enemy << "\n";
            }
            
            command_processed = true;
        }
        else if (cmd.action == "save") {
            save_requested = true;
            renderer.RenderMessage("Save requested.");
            continue;
        }
        else if (cmd.action == "load") {
            load_requested = true;
            renderer.RenderMessage("Load requested.");
            continue;
        }
        else if (cmd.action == "quit") {
            quit_requested = true;
            renderer.RenderMessage("Quitting game.");
            break;
        }
        else if (cmd.action == "help") {
            renderer.RenderHelp();
            command_processed = true;
        }
        
        if (turn_used) {
            MoveEnemies();
            ProcessBuildings();
            UpdateTowers();
            UpdateAllies();
        }
    }
}



bool Field::IsPlayerAlive() const {
    return player_->GetHealth() > 0;
}

bool Field::IsPlayerSlowed() const {
    return player_slowed_;
}

void Field::ResetPlayerSlowed() {
    player_slowed_ = false;
}

void Field::PlayerSwitchMode() {
    player_->SwitchFightMode();
}

int Field::GetPlayerX() const {
    return player_x_;
}

int Field::GetPlayerY() const {
    return player_y_;
}

Player* Field::GetPlayer() const {
    return player_.get();
}

Enemy* Field::GetEnemyAt(int x, int y) const {
    for (const auto& e : enemies_) {
        if (e->GetX() == x && e->GetY() == y) return e.get();
    }
    return nullptr;
}

Building* Field::GetBuildingAt(int x, int y) const {
    for (const auto& b : buildings_) {
        if (b->GetX() == x && b->GetY() == y) return b.get();
    }
    return nullptr;
}

void Field::RemoveEnemy(Enemy* enemy) {
    auto it = std::find_if(enemies_.begin(), enemies_.end(), [enemy](const std::unique_ptr<Enemy>& e) { return e.get() == enemy; });
    if (it != enemies_.end()) {
        size_t i = it - enemies_.begin();
        int ex = enemy_positions_[i].first;
        int ey = enemy_positions_[i].second;
        grid_[ey][ex].SetType(under_enemy_type_[i]);
        enemies_.erase(it);
        enemy_positions_.erase(enemy_positions_.begin() + i);
        under_enemy_type_.erase(under_enemy_type_.begin() + i);
        enemy_slowed_.erase(enemy_slowed_.begin() + i);
    }
}

void Field::RemoveBuilding(Building* building) {
    auto it = std::find_if(buildings_.begin(), buildings_.end(), [building](const std::unique_ptr<Building>& b) { return b.get() == building; });
    if (it != buildings_.end()) {
        size_t i = it - buildings_.begin();
        int bx = building_positions_[i].first;
        int by = building_positions_[i].second;
        grid_[by][bx].SetType(CellType::kEmpty);
        buildings_.erase(it);
        building_positions_.erase(building_positions_.begin() + i);
    }
}

void Field::AddTrap(int x, int y, int damage) {
    CellType original = grid_[y][x].GetType();
    traps_.emplace_back(std::make_unique<Trap>(x, y, damage));
    trap_positions_.emplace_back(x, y);
    under_trap_type_.push_back(original);
    grid_[y][x].SetType(CellType::kTrap);
}

void Field::UpdateTraps() {
    for (auto it = traps_.begin(); it != traps_.end(); ) {
        int tx = (*it)->GetX();
        int ty = (*it)->GetY();
        
        bool triggered = false;
        if (grid_[ty][tx].GetType() == CellType::kPlayer) {
            player_->TakeDamage((*it)->GetDamage());
            std::cout << "Trap at (" << tx << ", " << ty << ") damaged player for " << (*it)->GetDamage() << " damage\n";
            if (!player_->IsAlive()) {
                std::cout << "Player is dead!\n";
                return;
            }
            triggered = true;
        } else if (grid_[ty][tx].GetType() == CellType::kEnemy) {
            for (auto& enemy : enemies_) {
                if (enemy->GetX() == tx && enemy->GetY() == ty && enemy->IsAlive()) {
                    enemy->TakeDamage((*it)->GetDamage());
                    std::cout << "Trap at (" << tx << ", " << ty << ") damaged enemy for " << (*it)->GetDamage() << " damage\n";
                    if (!enemy->IsAlive()) {
                        RemoveEnemy(enemy.get());
                    }
                    triggered = true;
                    break;
                }
            }
        } else if (grid_[ty][tx].GetType() == CellType::kAlly) {
            for (auto& ally : allies_) {
                if (ally->GetX() == tx && ally->GetY() == ty && ally->IsAlive()) {
                    ally->TakeDamage((*it)->GetDamage());
                    std::cout << "Trap at (" << tx << ", " << ty << ") damaged ally for " << (*it)->GetDamage() << " damage\n";
                    if (!ally->IsAlive()) {
                        RemoveAlly(ally.get());
                    }
                    triggered = true;
                    break;
                }
            }
        }
        
        if (triggered) {
            grid_[ty][tx].SetType(under_trap_type_[it - traps_.begin()]);
            it = traps_.erase(it);
            trap_positions_.erase(trap_positions_.begin() + (it - traps_.begin()));
            under_trap_type_.erase(under_trap_type_.begin() + (it - traps_.begin()));
        } else {
            ++it;
        }
    }
}

void Field::UpdateTowers() {
    for (auto it = towers_.begin(); it != towers_.end(); ) {
        (*it)->Update(*this, *player_);
        if ((*it)->IsDestroyed()) {
            RemoveTower(it->get());
            it = towers_.erase(it);
        } else {
            ++it;
        }
    }
}

void Field::AddAlly(int x, int y) {
    if (x >= 0 && x < width_ && y >= 0 && y < height_ && grid_[y][x].GetType() == CellType::kEmpty) {
        allies_.emplace_back(std::make_unique<Ally>(x, y, 50, 10));
        ally_positions_.emplace_back(x, y);
        under_ally_type_.emplace_back(grid_[y][x].GetType());
        grid_[y][x].SetType(CellType::kAlly);
    }
}


void Field::UpdateAllies() {
    for (auto& a : allies_) {
        a->Update(this, player_.get());
    }
    for (size_t i = 0; i < allies_.size(); ++i) {
        ally_positions_[i].first = allies_[i]->GetX();
        ally_positions_[i].second = allies_[i]->GetY();
    }
}

Ally* Field::GetAllyAt(int x, int y) const {
    for (const auto& a : allies_) {
        if (a->GetX() == x && a->GetY() == y) return a.get();
    }
    return nullptr;
}