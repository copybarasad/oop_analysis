#include "Cell.h"

Cell::Cell(CellType type) : type_(type), hasTrap_(false) {}

CellType Cell::getType() const { return type_; }
bool Cell::isPassable() const { return type_ != CellType::WALL; }
bool Cell::isSlow() const { return type_ == CellType::SLOW; }
bool Cell::hasTrap() const { return hasTrap_; }
void Cell::setTrap(bool trap) { hasTrap_ = trap; }

std::shared_ptr<Player> Cell::getPlayer() const { return player_; }
void Cell::setPlayer(std::shared_ptr<Player> p) { player_ = p; }

std::shared_ptr<Enemy> Cell::getEnemy() const { return enemy_; }
void Cell::setEnemy(std::shared_ptr<Enemy> e) { enemy_ = e; }

std::shared_ptr<EnemyBase> Cell::getBase() const { return base_; }
void Cell::setBase(std::shared_ptr<EnemyBase> b) { base_ = b; }

std::shared_ptr<EnemyTower> Cell::getTower() const { return tower_; }
void Cell::setTower(std::shared_ptr<EnemyTower> t) { tower_ = t; }

std::shared_ptr<Ally> Cell::getAlly() const { return ally_; }
void Cell::setAlly(std::shared_ptr<Ally> a) { ally_ = a; }

bool Cell::isEmpty() const {
    return !player_ && !enemy_ && !base_ && !tower_ && !ally_;
}