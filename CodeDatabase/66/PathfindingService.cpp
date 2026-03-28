#include "../include/PathfindingService.h"
#include <queue>
#include <map>

bool PathfindingService::isPositionBlocked(const Position& pos, const Position& ignorePos, 
                                          const MapGrid& grid, const EnemyManager& enemyManager) const {
    if (!grid.canMoveTo(pos)) {
        return true;
    }
    
    if (pos == ignorePos) {
        return false;
    }
    
    return enemyManager.isPositionOccupiedByEnemy(pos);
}

Direction PathfindingService::findBestMoveTowardsTarget(const Position& start, const Position& target, 
                                                        const MapGrid& grid, const EnemyManager& enemyManager) const {
    std::queue<Position> queue;
    std::map<std::pair<int, int>, std::pair<int, int>> cameFrom;
    
    queue.push(target);
    cameFrom[{target.getX(), target.getY()}] = {-1, -1};
    
    std::vector<std::pair<int, int>> directions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    
    while (!queue.empty()) {
        Position current = queue.front();
        queue.pop();
        
        if (current == start) {
            std::pair<int, int> step = cameFrom[{start.getX(), start.getY()}];
            
            int dx = step.first - start.getX();
            int dy = step.second - start.getY();
            
            if (dx == 1) return Direction::RIGHT;
            if (dx == -1) return Direction::LEFT;
            if (dy == 1) return Direction::DOWN;
            if (dy == -1) return Direction::UP;
            
            return Direction::NONE;
        }
        
        for (const auto& dir : directions) {
            Position next(current.getX() + dir.first, current.getY() + dir.second);
            
            if (isPositionBlocked(next, start, grid, enemyManager)) {
                continue;
            }
            
            std::pair<int, int> nextKey = {next.getX(), next.getY()};
            if (cameFrom.find(nextKey) == cameFrom.end()) {
                queue.push(next);
                cameFrom[nextKey] = {current.getX(), current.getY()};
            }
        }
    }
    
    return Direction::NONE;
}
