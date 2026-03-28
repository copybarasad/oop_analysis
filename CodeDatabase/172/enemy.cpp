#include "enemy.h"

#include <random>
#include <vector>

namespace game {
    // ------------ Verify possible position --------------
    bool Enemy::canMoveTo(Pos position, int w, int h,
                          const std::vector<std::vector<Cell>>& cells) const {
        return (position.x >= 0 && position.x < w &&
                position.y >= 0 && position.y < h &&
                cells[position.y][position.x].isWalkable());
    }

    // --------------- Random enemy move ------------------
    // Using for balance enemy moving (not simple pursuit)
    Pos Enemy::moveRandom(std::mt19937& rng,
                           int w, int h,
                           Character& player,
                           const std::vector<std::vector<Cell>>& cells,
                           const std::vector<Pos>& occupiedPositions)
    {
        const std::vector<Direction> dirs = { 
            Direction::Left, Direction::Up, Direction::Right, Direction::Down
        };

                
        // Try to attack Player
        for (Direction dir: dirs) {
            Pos delta = Pos::toPos(dir);

            int nx = position_.x + delta.x;
            int ny = position_.y + delta.y;

            if (nx == player.getPosition().x && ny == player.getPosition().y) {
                player.takeDamage(damage_);
                return position_;   // no movement
            }
                    
        }

                
        // Look for all available directs for Enemy
        std::vector<Direction> availableDirs;
        for (Direction dir: dirs) {
            Pos delta = Pos::toPos(dir);

            int nx = position_.x + delta.x;
            int ny = position_.y + delta.y;

            if (canMoveTo(Pos(nx, ny), w, h, cells)) {
                availableDirs.push_back(dir);
            }
        }

        if (availableDirs.empty()) return position_; // no movement

        std::uniform_int_distribution<int> dist(0, (int)availableDirs.size() - 1);   // a.k.a. random.randint()
        Direction choice = availableDirs[dist(rng)];

        Pos randomDir = Pos::toPos(choice);
        Pos targetPos = {position_.x + randomDir.x, position_.y + randomDir.y};

        for (const Pos& pos: occupiedPositions) {
            if (pos == targetPos) {
                return position_;   // no movement
            }
        }

        position_.x = targetPos.x;
        position_.y = targetPos.y;
                
        return position_;   // new position
    }

    // -------------- Enemy move to player ----------------
    //                  Simple pursuit
    Pos Enemy::moveToPlayer(int w, int h,
                      Character& player,
                      const std::vector<std::vector<Cell>>& cells,
                      const std::vector<Pos>& occupiedPositions)
    {
        Pos playerPos = player.getPosition();

        int dx = playerPos.x - position_.x;
        int dy = playerPos.y - position_.y;
                
        // Horizontal movement
        if (dx != 0) {
            int nx = position_.x + (dx > 0 ? 1 : -1);
            if (nx == playerPos.x && position_.y == playerPos.y) {
                player.takeDamage(damage_);
                return position_;   // no movement
            }

            Pos targetPos = {nx, position_.y};
            bool occupied = false;
            for (const Pos& pos: occupiedPositions) {
                if (pos == targetPos) {
                    occupied = true;
                    break;
                }
            }

            if (!occupied && nx >= 0 && nx < w && cells[position_.y][nx].isWalkable()) {
                position_.x = nx;
                return position_;
            }
        }

        // Vertical movement
        if (dy != 0) {
            int ny = position_.y + (dy > 0 ? 1 : -1);
            if (position_.x == playerPos.x && ny == playerPos.y) {
                player.takeDamage(damage_);
                return position_; // no movement
            }

            Pos targetPos = {position_.x, ny};
            bool occupied = false;
            for (const Pos& pos: occupiedPositions) {
                if (pos == targetPos) {
                    occupied = true;
                    break;
                }
            }

            if (!occupied && ny >= 0 && ny < h && cells[ny][position_.x].isWalkable()) {
                position_.y = ny;
                return position_;
            }
        }

        return position_;   // no movement
    }
}