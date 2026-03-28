#include "GameField.h"
#include "Player.h"
#include <iostream>
#include <algorithm>

GameField::GameField(int width, int height) : width_(width), height_(height) {}

bool GameField::isInside(int x, int y) const {
    return x >= 0 && y >= 0 && x < width_ && y < height_;
}

void GameField::addUnit(std::unique_ptr<Unit> u) {
    if (!u) return;

    // Если игрок задан и координаты совпадают с позицией игрока,
    // пытаемся найти ближайшую свободную клетку для нового юнита.
    if (player_) {
        if (u->x() == player_->x() && u->y() == player_->y()) {
            // Найти ближайшую свободную клетку (по Chebyshev-радиусу от 1 до max)
            bool placed = false;
            for (int r = 1; r <= std::max(width_, height_) && !placed; ++r) {
                for (int dx = -r; dx <= r && !placed; ++dx) {
                    for (int dy = -r; dy <= r && !placed; ++dy) {
                        int nx = player_->x() + dx;
                        int ny = player_->y() + dy;
                        if (!isInside(nx, ny)) continue;
                        if (std::abs(dx) > r || std::abs(dy) > r) continue;
                        // клетка должна быть свободна от юнитов, зданий и ловушек
                        if (!unitAt(nx, ny) && !buildingAt(nx, ny)) {
                            u->setPosition(nx, ny);
                            placed = true;
                        }
                    }
                }
            }
            if (!placed) {
                std::cout << "Warning: cannot place unit away from player; placing on same cell.\n";
            }
        }
    }

    units_.push_back(std::move(u));
}

Unit* GameField::unitAt(int x, int y) {
    for (auto& u : units_) {
        if (!u->isDead() && u->x() == x && u->y() == y) return u.get();
    }
    return nullptr;
}

const Unit* GameField::unitAt(int x, int y) const {
    for (const auto& u : units_) {
        if (!u->isDead() && u->x() == x && u->y() == y) return u.get();
    }
    return nullptr;
}

void GameField::removeDeadUnits() {
    units_.erase(std::remove_if(units_.begin(), units_.end(), [this](const std::unique_ptr<Unit>& u) {
        if (u->isDead()) {
            if (u->isEnemy() && player_) player_->onEnemyKilled();
            return true;
        }
        return false;
    }), units_.end());
}

void GameField::addBuilding(std::unique_ptr<Building> b) {
    buildings_.push_back(std::move(b));
}

Building* GameField::buildingAt(int x, int y) {
    for (auto& b : buildings_) {
        if (!b->isDestroyed() && b->x() == x && b->y() == y) return b.get();
    }
    return nullptr;
}


const Building* GameField::buildingAt(int x, int y) const {
    for (const auto& b : buildings_) {
        if (!b->isDestroyed() && b->x() == x && b->y() == y) return b.get();
    }
    return nullptr;
}

bool GameField::placeTrap(int x, int y, int damage) {
    if (!isInside(x, y)) return false;
    if (unitAt(x, y) || buildingAt(x, y)) return false;
    for (auto& t : traps_) if (t.x == x && t.y == y) return false;
    traps_.push_back({x, y, damage});
    return true;
}

void GameField::checkTrapTrigger(int x, int y) {
    for (size_t i = 0; i < traps_.size(); ++i) {
        if (traps_[i].x == x && traps_[i].y == y) {
            Unit* u = unitAt(x, y);
            if (u && u->isEnemy()) {
                u->takeDamage(traps_[i].damage);
            } else {
                std::cout << "Trap triggered at ("<<x<<","<<y<<") but no unit.\n";
            }
            traps_.erase(traps_.begin() + i);
            return;
        }
    }
}

void GameField::tick() {
    // 1) удаляем мёртвых юнитов и даём игроку возможные бонусы
    removeDeadUnits();

    // 1.5) даём юнитам выполнить свои действия
    for (auto& u : units_) {
        if (!u->isDead()) {
            u->act(*this);
        }
    }

    // 2) вызываем поведение зданий — только если у нас задан игрок,
    //    иначе будет UB при deref player_. Это защитный патч.
    if (player_) {
        for (auto& b : buildings_) {
            if (!b->isDestroyed()) {
                b->tryAttack(*player_, *this);
                b->tick();
            }
        }
    } else {
        for (auto& b : buildings_) {
            if (!b->isDestroyed()) b->tick();
        }
    }
}


void GameField::debugPrint() const {
    std::cout << "Field " << width_ << "x" << height_ << "\n";
    std::cout << "Units:\n";
    for (auto& u : units_) {
        std::cout << " - ("<<u->x()<<","<<u->y()<<") hp="<<u->hp() << " enemy="<<u->isEnemy()<<"\n";
    }
    std::cout << "Buildings:\n";
    for (auto& b : buildings_) {
        std::cout << " - ("<<b->x()<<","<<b->y()<<")\n";
    }
    std::cout << "Traps: " << traps_.size() << "\n";
}
