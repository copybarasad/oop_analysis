#include "EnemyConstruction.hpp"

EnemyConstruction::EnemyConstruction(int rate, Position pos, Grid& grid, Logger& logger)
    : hp_(50), spawnRate_(rate), pos_(pos), grid_(grid), logger_(logger) {}

std::unique_ptr<Enemy> EnemyConstruction::spawnNewEnemy() {
    struct offsets{int dx,dy;};
    std::vector<offsets> direction = {{1,0},{0,1},{-1,0},{0,-1}};
    for(auto& dir: direction){
	int x = pos_.x + dir.dx;
	int y = pos_.y + dir.dy;
	if(grid_.IsFreeCell(x,y)){
	    logger_.info("Construction spawned new enemy!");
	    return std::make_unique<Enemy>(25, 5, Position{pos_.x+1,pos_.y}, logger_);
	}
    }
    logger_.warning("path is blocked! cannot spawn enemy!");
    return nullptr;
}

bool EnemyConstruction::isAlive() const { return hp_ > 0; }
void EnemyConstruction::takeDamage(int damage) { hp_ -= damage; }
int EnemyConstruction::getRate() const { return spawnRate_; }
Position EnemyConstruction::getPos() const { return pos_; }

void EnemyConstruction::save(std::ostream& os) const {
    os << hp_ << " " << spawnRate_ << " " << pos_.x << " " << pos_.y << "\n";
}

void EnemyConstruction::load(std::istream& is) {
    is >> hp_ >> spawnRate_ >> pos_.x >> pos_.y;
}
