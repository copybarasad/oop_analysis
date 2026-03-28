#include "enemy_building.h"
#include "../map/game_map.h"
#include "../logging/event_manager.h"
#include <sstream>

EnemyBuilding::EnemyBuilding(int start_x, int start_y, int interval) 
    : x(start_x), y(start_y), health(DEFAULT_HEALTH),
      spawn_interval(interval), spawn_cooldown(interval) {}

EnemyBuilding::EnemyBuilding(const EnemyBuilding& other)
    : x(other.x), y(other.y), health(other.health),
      spawn_interval(other.spawn_interval), 
      spawn_cooldown(other.spawn_cooldown) {}

int EnemyBuilding::get_x() const { return x; }
int EnemyBuilding::get_y() const { return y; }
int EnemyBuilding::get_health() const { return health; }
int EnemyBuilding::get_spawn_interval() const { return spawn_interval; }
int EnemyBuilding::get_spawn_cooldown() const { return spawn_cooldown; }

bool EnemyBuilding::is_alive() const
{
    return health > 0;
}

void EnemyBuilding::take_damage(int spell_damage)
{
    health = std::max(0, health - spell_damage);
    
    std::ostringstream oss;
    oss << "Building at (" << x << "," << y << ") took " << spell_damage << " damage, health: " << health;
    EventManager::get_instance().notify(GameEvent(GameEventType::BUILDING_DAMAGED, oss.str()));
}

void EnemyBuilding::update()
{
    if (spawn_cooldown > 0)
    {
        spawn_cooldown--;
    }
}

void EnemyBuilding::reset_cooldown()
{
    spawn_cooldown = spawn_interval;
}

bool EnemyBuilding::can_spawn() const
{
    return spawn_cooldown == 0;
}

bool EnemyBuilding::spawn_enemy(GameMap& map, int spawn_x, int spawn_y)
{
    if (!map.is_valid_position(spawn_x, spawn_y) ||
        !map.get_cell(spawn_x, spawn_y).get_availability() ||
        map.get_cell(spawn_x, spawn_y).get_type() == CellType::PLAYER)
    {
        return false;
    }

    map.get_enemies_mutable().emplace_back(spawn_x, spawn_y);
    map.get_cell(spawn_x, spawn_y).set_type(CellType::ENEMY);
    map.get_cell(spawn_x, spawn_y).set_availability(false);
    
    std::ostringstream oss;
    oss << "Building at (" << x << "," << y << ") spawned enemy at (" << spawn_x << "," << spawn_y << ")";
    EventManager::get_instance().notify(GameEvent(GameEventType::BUILDING_SPAWNED_ENEMY, oss.str()));
    
    return true;
}

bool EnemyBuilding::spawn_enemy_near_building(GameMap& map)
{
    if (!map.get_enemy_building() || !map.get_enemy_building()->is_alive()) return false;
    
    int building_x = map.get_enemy_building()->get_x();
    int building_y = map.get_enemy_building()->get_y();
    
    int directions[8][2] =
    {
        {-1, -1}, {0, -1}, {1, -1},
        {-1,  0},          {1,  0},
        {-1,  1}, {0,  1}, {1,  1}
    };
    
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < 7; ++i)
    {
        int j = std::uniform_int_distribution<>(i, 7)(gen);
        std::swap(directions[i], directions[j]);
    }
    
    for (const auto& dir : directions)
    {
        int spawn_x = building_x + dir[0];
        int spawn_y = building_y + dir[1];
        
        if (spawn_enemy(map, spawn_x, spawn_y)) return true;
    }
    
    return false;
}

void EnemyBuilding::save_binary(std::ostream& os) const
{
    os.write(reinterpret_cast<const char*>(&x), sizeof(x));
    os.write(reinterpret_cast<const char*>(&y), sizeof(y));
    os.write(reinterpret_cast<const char*>(&health), sizeof(health));
    os.write(reinterpret_cast<const char*>(&spawn_interval), sizeof(spawn_interval));
    os.write(reinterpret_cast<const char*>(&spawn_cooldown), sizeof(spawn_cooldown));
}

void EnemyBuilding::load_binary(std::istream& is)
{
    is.read(reinterpret_cast<char*>(&x), sizeof(x));
    is.read(reinterpret_cast<char*>(&y), sizeof(y));
    is.read(reinterpret_cast<char*>(&health), sizeof(health));
    is.read(reinterpret_cast<char*>(&spawn_interval), sizeof(spawn_interval));
    is.read(reinterpret_cast<char*>(&spawn_cooldown), sizeof(spawn_cooldown));
}