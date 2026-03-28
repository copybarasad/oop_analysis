#include "Enemy.h"
#include "GameField.h"
#include "Player.h"
#include <queue>
#include <vector>
#include <algorithm>

Enemy::Enemy(int x, int y, const std::string& name, int hp, int dmg) 
    : Entity(x, y, hp), name(name), dmg(dmg) {}

std::string Enemy::getName() const {
    return name;
}

int Enemy::getHp() const { return Entity::getHp(); }

int Enemy::getDamage() const {
    return dmg;
}

 

bool Enemy::attackPlayer(Player* player) {
    if (!player || !isAlive()) return false;
    
    int dx = abs(player->getX() - x);
    int dy = abs(player->getY() - y);
    int distance = dx + dy;
    
    if (distance == 1) {
        player->takeDamage(dmg);
        return true;
    }
    
    return false;
}

 

void Enemy::moveTowardsPlayer(int player_x, int player_y, GameField& field) {
    std::queue<std::pair<int, int>> queue;
    std::vector<std::vector<bool>> visited(field.getHeight(), std::vector<bool>(field.getWidth(), false));
    std::vector<std::vector<std::pair<int, int>>> parent(field.getHeight(), std::vector<std::pair<int, int>>(field.getWidth(), {-1, -1}));
    
    queue.push({x, y});
    visited[y][x] = true;
    
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    
    bool found = false;
    std::pair<int, int> target = {-1, -1};
    
    while (!queue.empty() && !found) {
        auto current = queue.front();
        queue.pop();
        
        if (current.first == player_x && current.second == player_y) {
            found = true;
            target = current;
            break;
        }
        
        for (int i = 0; i < 4; i++) {
            int nx = current.first + dx[i];
            int ny = current.second + dy[i];
            
            if (field.isPositionValid(nx, ny) && !visited[ny][nx] && 
                field.getCellType(nx, ny) != CellType::IMPASSABLE && 
                !field.hasEnemyAt(nx, ny) && !field.hasBuildingAt(nx, ny) && !field.hasTowerAt(nx, ny)) {
                
                visited[ny][nx] = true;
                parent[ny][nx] = current;
                queue.push({nx, ny});
            }
        }
    }
    
    if (found) {
        std::vector<std::pair<int, int>> path;
        std::pair<int, int> current = target;
        
        while (current.first != -1 && current.second != -1) {
            path.push_back(current);
            current = parent[current.second][current.first];
        }
        
        if (path.size() > 1) {
            auto next_pos = path[path.size() - 2];
            
            if (next_pos.first == player_x && next_pos.second == player_y) {
                return;
            }
            
            setPosition(next_pos.first, next_pos.second);
        }
    }
}
