#include "enemy.h"
#include "../map/game_map.h"
#include "../logging/event_manager.h"
#include <sstream>

bool Enemy::find_next_move_dijkstra(GameMap& map, int start_x, int start_y, int target_x, int target_y, int& next_x, int& next_y)
{
    if (start_x == target_x && start_y == target_y) return false;

    struct CellInfo
    {
        int x, y;
        int distance;
        CellInfo(int x, int y, int dist) : x(x), y(y), distance(dist) {}
        
        bool operator>(const CellInfo& other) const { return distance > other.distance; }
    };

    std::vector<std::vector<int>> distance(map.get_height(), std::vector<int>(map.get_width(), map.get_width() * map.get_height()));
    std::vector<std::vector<std::pair<int, int>>> parent(map.get_height(), std::vector<std::pair<int, int>>(map.get_width(), {-1, -1}));
    std::vector<std::vector<bool>> visited(map.get_height(), std::vector<bool>(map.get_width(), false));

    std::priority_queue<CellInfo, std::vector<CellInfo>, std::greater<CellInfo>> pq;

    distance[start_y][start_x] = 0;
    pq.push(CellInfo(start_x, start_y, 0));

    int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    while (!pq.empty())
    {
        CellInfo current = pq.top();
        pq.pop();

        int current_x = current.x;
        int current_y = current.y;

        if (visited[current_y][current_x]) continue;

        visited[current_y][current_x] = true;

        if (current_x == target_x && current_y == target_y) break;

        for (const auto& dir : directions)
        {
            int neighbor_x = current_x + dir[0];
            int neighbor_y = current_y + dir[1];

            if (map.is_valid_position(neighbor_x, neighbor_y) && map.get_cell(neighbor_x, neighbor_y).get_availability())
            {
                int new_distance = distance[current_y][current_x] + 1;

                if (new_distance < distance[neighbor_y][neighbor_x])
                {
                    distance[neighbor_y][neighbor_x] = new_distance;
                    parent[neighbor_y][neighbor_x] = {current_x, current_y};
                    pq.push(CellInfo(neighbor_x, neighbor_y, new_distance));
                }
            }
        }
    }

    if (parent[target_y][target_x].first == -1) return false;

    std::pair<int, int> first_move = reconstruct_first_move(target_x, target_y, start_x, start_y, parent);
    next_x = first_move.first;
    next_y = first_move.second;

    return true;
}

std::pair<int, int> Enemy::reconstruct_first_move(int target_x, int target_y,
    int start_x, int start_y, const std::vector<std::vector<std::pair<int, int>>>& parent)
{
    int current_x = target_x;
    int current_y = target_y;
    int prev_x = parent[current_y][current_x].first;
    int prev_y = parent[current_y][current_x].second;

    while (!(prev_x == start_x && prev_y == start_y))
    {
        current_x = prev_x;
        current_y = prev_y;
        prev_x = parent[current_y][current_x].first;
        prev_y = parent[current_y][current_x].second;
    }

    return {current_x, current_y};
}

Enemy::Enemy(int start_x, int start_y)
    : x(start_x), y(start_y), health(DEFAULT_HEALTH), damage(DEFAULT_DAMAGE) {}

int Enemy::get_x() const { return x; }
int Enemy::get_y() const { return y; }
int Enemy::get_health() const { return health; }
int Enemy::get_damage() const { return damage; }

void Enemy::set_position(int new_x, int new_y)
{
    x = new_x;
    y = new_y;
}

bool Enemy::is_alive() const
{
    return health > 0;
}

void Enemy::take_damage(int player_damage)
{
    health = std::max(0, health - player_damage);
}

bool Enemy::move(GameMap& map, size_t enemy_index)
{
    if (enemy_index >= map.get_enemies().size()) return false;

    int player_x = map.get_player()->get_x();
    int player_y = map.get_player()->get_y();
    int enemy_x = map.get_enemies()[enemy_index].get_x();
    int enemy_y = map.get_enemies()[enemy_index].get_y();

    int new_x, new_y;
    
    if (find_next_move_dijkstra(map, enemy_x, enemy_y, player_x, player_y, new_x, new_y))
    {
        if (map.get_cell(new_x, new_y).get_type() == CellType::PLAYER)
        {
            map.get_player()->take_damage(map.get_enemies()[enemy_index].get_damage());
            return true;
        }

        if (map.get_cell(new_x, new_y).get_trap_spell())
        {
            take_damage(DAMAGE_BY_TRAP_SPELL);
            if (!is_alive())
            {
                map.get_cell(new_x, new_y).set_trap_spell(false);
                map.get_cell(x, y).set_availability(true);
                map.get_cell(x, y).set_type(CellType::EMPTY);
                return true;
            }
        }

        Enemy& enemy = map.get_enemies_mutable()[enemy_index];
        int old_x = enemy.get_x();
        int old_y = enemy.get_y();

        map.get_cell(enemy.get_x(), enemy.get_y()).set_type(CellType::EMPTY);
        map.get_cell(enemy.get_x(), enemy.get_y()).set_availability(true);

        enemy.set_position(new_x, new_y);
        map.get_cell(new_x, new_y).set_type(CellType::ENEMY);
        map.get_cell(new_x, new_y).set_availability(false);
        map.get_cell(new_x, new_y).set_trap_spell(false);

        std::ostringstream oss;
        oss << "Enemy moved from (" << old_x << "," << old_y << ") to (" << new_x << "," << new_y << ")";
        EventManager::get_instance().notify(GameEvent(GameEventType::ENEMY_MOVED, oss.str()));

        return true;
    }

    return false;
}

void Enemy::save_binary(std::ostream& os) const
{
    os.write(reinterpret_cast<const char*>(&x), sizeof(x));
    os.write(reinterpret_cast<const char*>(&y), sizeof(y));
    os.write(reinterpret_cast<const char*>(&health), sizeof(health));
    os.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
}

void Enemy::load_binary(std::istream& is)
{
    is.read(reinterpret_cast<char*>(&x), sizeof(x));
    is.read(reinterpret_cast<char*>(&y), sizeof(y));
    is.read(reinterpret_cast<char*>(&health), sizeof(health));
    is.read(reinterpret_cast<char*>(&damage), sizeof(damage));
}