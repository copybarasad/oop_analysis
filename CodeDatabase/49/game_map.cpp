#include "game_map.h"

void GameMap::initialize_cells()
{
    cells.resize(height, std::vector<Cell>(width));
}

void GameMap::generate_enemies()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> x_dis(0, width - 1);
    std::uniform_int_distribution<> y_dis(0, height - 1);
    
    int base = INITIAL_ENEMY_COUNT;
    int level_increase = level - 1;
    int enemies_to_spawn = base + level_increase;

    int player_x = player->get_x();
    int player_y = player->get_y();
    
    while (enemies_to_spawn > 0)
    {
        int x = x_dis(gen);
        int y = y_dis(gen);
        
        if (cells[y][x].get_type() == CellType::EMPTY)
        {
            int distance = std::abs(x - player_x) + std::abs(y - player_y);
            
            if (distance >= MIN_SPAWN_DISTANCE_FROM_PLAYER)
            {
                enemies.emplace_back(x, y);
                cells[y][x].set_type(CellType::ENEMY);
                cells[y][x].set_availability(false);
                enemies_to_spawn--;
            }
        }
    }
}

void GameMap::generate_enemy_tower()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> x_dis(0, width - 1);
    std::uniform_int_distribution<> y_dis(0, height - 1);

    int player_x = player->get_x();
    int player_y = player->get_y();
    
    bool tower_spawn = false;

    while (!tower_spawn)
    {
        int x = x_dis(gen);
        int y = y_dis(gen);
            
        if (cells[y][x].get_type() == CellType::EMPTY)
        {
            int distance = std::abs(x - player_x) + std::abs(y - player_y);
                
            if (distance >= MIN_TOWER_DISTANCE_FROM_PLAYER)
            {
                enemy_tower = new EnemyTower(x, y);
                cells[y][x].set_type(CellType::ENEMY_TOWER);
                cells[y][x].set_availability(false);
                tower_spawn = true;
            }
        }
    }
}

void GameMap::generate_enemy_building()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> x_dis(0, width - 1);
    std::uniform_int_distribution<> y_dis(0, height - 1);

    int player_x = player->get_x();
    int player_y = player->get_y();
    int tower_x = enemy_tower->get_x();
    int tower_y = enemy_tower->get_y();
    
    bool building_spawn = false;

    while (!building_spawn)
    {
        int x = x_dis(gen);
        int y = y_dis(gen);
            
        if (cells[y][x].get_type() == CellType::EMPTY)
        {
            int distance_to_player = std::abs(x - player_x) + std::abs(y - player_y);
            int distance_to_tower = std::abs(x - tower_x) + std::abs(y - tower_y);
                
            if (distance_to_player >= MIN_BUILDING_DISTANCE_FROM_PLAYER &&
                distance_to_tower >= MIN_BUILDING_DISTANCE_FROM_TOWER)
            {
                enemy_building = new EnemyBuilding(x, y, BUILDING_SPAWN_INTERVAL);
                cells[y][x].set_type(CellType::ENEMY_BUILDING);
                cells[y][x].set_availability(false);
                building_spawn = true;
            }
        }
    }
}

void GameMap::generate_walls()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 100);
    
    const int wall_probability = WALL_PROBABILITY;

    int player_x = player->get_x();
    int player_y = player->get_y();

    int building_x = enemy_building->get_x();
    int building_y = enemy_building->get_y();

    int tower_x = enemy_tower->get_x();
    int tower_y = enemy_tower->get_y();
    
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (cells[y][x].get_type() != CellType::EMPTY)
            {
                continue;
            }

            int player_distance = std::abs(x - player_x) + std::abs(y - player_y);
            int building_distance = std::abs(x - building_x) + std::abs(y - building_y);
            int tower_distance = std::abs(x - tower_x) + std::abs(y - tower_y);

            if (dis(gen) < wall_probability &&
                player_distance >= MIN_SPAWN_DISTANCE_FROM_PLAYER &&
                building_distance >= MIN_SPAWN_DISTANCE_FROM_PLAYER &&
                tower_distance >= MIN_SPAWN_DISTANCE_FROM_PLAYER)
            {
                cells[y][x].set_type(CellType::WALL);
                cells[y][x].set_availability(false);
            }
        }
    }
}

void GameMap::generate_slow_traps()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 100);
    
    const int trap_probability = TRAP_PROBABILITY;

    int player_x = player->get_x();
    int player_y = player->get_y();
    
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (cells[y][x].get_type() != CellType::EMPTY)
            {
                continue;
            }

            int distance = std::abs(x - player_x) + std::abs(y - player_y);

            if (dis(gen) < trap_probability && distance >= MIN_SPAWN_DISTANCE_FROM_PLAYER)
            {
                cells[y][x].set_slow_trap(true);
            }
        }
    }
}

GameMap::GameMap(int w, int h) : GameMap(w, h, 1) {}

GameMap::GameMap(int w, int h, int current_level) : width(w), height(h),
    level(current_level), player(nullptr), enemy_tower(nullptr), enemy_building(nullptr)
{
    initialize_cells();

    player = new Player(w / 2, h / 2);
    cells[player->get_y()][player->get_x()].set_type(CellType::PLAYER);

    generate_enemies();
    generate_enemy_tower();
    generate_enemy_building();
    generate_walls();
    generate_slow_traps();
}

GameMap::GameMap(int w, int h, int current_level, Player* existing_player)
    : width(w), height(h), level(current_level),
      player(existing_player), enemy_tower(nullptr), enemy_building(nullptr)
{
    initialize_cells();

    player->set_position(w / 2, h / 2);
    cells[player->get_y()][player->get_x()].set_type(CellType::PLAYER);

    generate_enemies();
    generate_enemy_tower();
    generate_enemy_building();
    generate_walls();
    generate_slow_traps();
}

GameMap::GameMap(const GameMap& other)
    : width(other.width), height(other.height), cells(other.cells), enemies(other.enemies)
{
    if (other.player)
    {
        player = new Player(*other.player);
    }
    else
    {
        player = nullptr;
    }
    
    if (other.enemy_tower)
    {
        enemy_tower = new EnemyTower(*other.enemy_tower);
    }
    else
    {
        enemy_tower = nullptr;
    }

    if (other.enemy_building)
    {
        enemy_building = new EnemyBuilding(*other.enemy_building);
    }
    else
    {
        enemy_building = nullptr;
    }
}

GameMap::GameMap(GameMap&& other) noexcept
    : width(other.width), height(other.height),
      cells(std::move(other.cells)),
      player(other.player),
      enemies(std::move(other.enemies)),
      enemy_tower(other.enemy_tower),
      enemy_building(other.enemy_building)
{
    other.width = 0;
    other.height = 0;
    other.player = nullptr;
    other.enemy_tower = nullptr;
    other.enemy_building = nullptr;
}

GameMap& GameMap::operator=(const GameMap& other)
{
    if (this != &other)
    {
        delete player;
        delete enemy_tower;
        delete enemy_building;
        
        width = other.width;
        height = other.height;
        cells = other.cells;
        enemies = other.enemies;
        
        if (other.player)
        {
            player = new Player(*other.player);
        }
        else
        {
            player = nullptr;
        }
        
        if (other.enemy_tower)
        {
            enemy_tower = new EnemyTower(*other.enemy_tower);
        }
        else
        {
            enemy_tower = nullptr;
        }

        if (other.enemy_building)
        {
            enemy_building = new EnemyBuilding(*other.enemy_building);
        }
        else
        {
            enemy_building = nullptr;
        }
    }
    
    return *this;
}

GameMap& GameMap::operator=(GameMap&& other) noexcept
{
    if (this != &other)
    {
        delete player;
        delete enemy_tower;
        delete enemy_building;
        
        width = other.width;
        height = other.height;
        cells = std::move(other.cells);
        player = other.player;
        enemies = std::move(other.enemies);
        enemy_tower = other.enemy_tower;
        enemy_building = other.enemy_building;
        
        other.width = 0;
        other.height = 0;
        other.player = nullptr;
        other.enemy_tower = nullptr;
        other.enemy_building = nullptr;
    }

    return *this;
}

GameMap::~GameMap()
{
    delete player;
    delete enemy_tower;
    delete enemy_building;
}

int GameMap::get_width() const { return width; }
int GameMap::get_height() const { return height; }
Player* GameMap::get_player() const { return player; }
const std::vector<Enemy>& GameMap::get_enemies() const { return enemies; }
std::vector<Enemy>& GameMap::get_enemies_mutable() { return enemies; }
EnemyTower* GameMap::get_enemy_tower() const { return enemy_tower; }
EnemyBuilding* GameMap::get_enemy_building() const { return enemy_building; }
Cell& GameMap::get_cell(int x, int y) { return cells[y][x]; }
const Cell& GameMap::get_cell(int x, int y) const { return cells[y][x]; }
const std::vector<std::vector<Cell>>& GameMap::get_cells() const { return cells; }

bool GameMap::is_valid_position(int x, int y) const
{
    return x >= 0 && x < width && y >= 0 && y < height;
}

void GameMap::remove_dead_enemies()
{
    enemies.erase(
    std::remove_if(enemies.begin(), enemies.end(),
        [](const Enemy& enemy) { return !enemy.is_alive(); }),
        enemies.end());
}

void GameMap::save_binary(std::ostream& os) const
{
    os.write(reinterpret_cast<const char*>(&width), sizeof(width));
    os.write(reinterpret_cast<const char*>(&height), sizeof(height));
    os.write(reinterpret_cast<const char*>(&level), sizeof(level));

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            cells[y][x].save_binary(os);
        }
    }

    bool has_player = (player != nullptr);
    os.write(reinterpret_cast<const char*>(&has_player), sizeof(has_player));
    if (has_player) player->save_binary(os);

    uint32_t count = static_cast<uint32_t>(enemies.size());
    os.write(reinterpret_cast<const char*>(&count), sizeof(count));
    for (const auto& e : enemies) e.save_binary(os);

    bool has_building = (enemy_building != nullptr);
    os.write(reinterpret_cast<const char*>(&has_building), sizeof(has_building));
    if (has_building) enemy_building->save_binary(os);

    bool has_tower = (enemy_tower != nullptr);
    os.write(reinterpret_cast<const char*>(&has_tower), sizeof(has_tower));
    if (has_tower) enemy_tower->save_binary(os);
}

void GameMap::load_binary(std::istream& is)
{
    is.read(reinterpret_cast<char*>(&width), sizeof(width));
    is.read(reinterpret_cast<char*>(&height), sizeof(height));
    is.read(reinterpret_cast<char*>(&level), sizeof(level));

    cells.assign(height, std::vector<Cell>(width));
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            cells[y][x].load_binary(is);
        }
    }       

    bool has_player = false;
    is.read(reinterpret_cast<char*>(&has_player), sizeof(has_player));
    if (has_player)
    {
        if (player) delete player;
        player = new Player(0,0);
        player->load_binary(is);
    }

    uint32_t count = 0;
    is.read(reinterpret_cast<char*>(&count), sizeof(count));
    enemies.clear();
    enemies.reserve(count);
    for (uint32_t i=0;i<count;++i)
    {
        Enemy e(0,0);
        e.load_binary(is);
        enemies.push_back(std::move(e));
    }

    bool has_building=false;
    is.read(reinterpret_cast<char*>(&has_building), sizeof(has_building));
    if (has_building)
    {
        if (enemy_building) delete enemy_building;
        enemy_building = new EnemyBuilding(0,0,0);
        enemy_building->load_binary(is);
    }

    bool has_tower=false;
    is.read(reinterpret_cast<char*>(&has_tower), sizeof(has_tower));
    if (has_tower)
    {
        if (enemy_tower) delete enemy_tower;
        enemy_tower = new EnemyTower(0,0);
        enemy_tower->load_binary(is);
    }
}