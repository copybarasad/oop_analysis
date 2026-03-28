#include "ally.h"
#include "field.h"  
#include <iostream>
#include <vector>
#include <cstdlib>  
#include <limits>  
#include <cmath>
#include <cassert>

Ally::Ally(int x, int y, int health, int damage) 
    : health_(health), damage_(damage), x_(x), y_(y), under_type_(CellType::kEmpty) {
    assert(health_ > 0 && "Health must be positive");
    assert(damage_ > 0 && "Damage must be positive");
}

int Ally::GetHealth() const { return health_; }

int Ally::GetDamage() const { return damage_; }

void Ally::TakeDamage(int damage) { 
    health_ -= damage; 
    if (health_ < 0) health_ = 0;
}

bool Ally::IsAlive() const { return health_ > 0; }

int Ally::GetX() const { return x_; }

int Ally::GetY() const { return y_; }

void Ally::SetPosition(int x, int y) { 
    x_ = x; 
    y_ = y; 
}

void Ally::Update(Field* field, Player* player) {
    struct Target {
        int x, y;
        enum Type { ENEMY, BUILDING, TOWER } type;
        void* ptr;
    };
    Target nearest_target = {-1, -1, Target::ENEMY, nullptr};
    int min_distance = INT_MAX;

    for (auto& enemy : field->GetEnemies()) {
        if (!enemy->IsAlive()) continue;
        int ex = enemy->GetX(), ey = enemy->GetY();
        int dx = std::abs(ex - x_), dy = std::abs(ey - y_);
        int distance = dx + dy;
        if (distance < min_distance) {
            min_distance = distance;
            nearest_target = {ex, ey, Target::ENEMY, enemy.get()};
        }
    }

    for (auto& building : field->GetBuildings()) {
        if (!building->IsAlive()) continue;
        int bx = building->GetX(), by = building->GetY();
        int dx = std::abs(bx - x_), dy = std::abs(by - y_);
        int distance = dx + dy;
        if (distance < min_distance) {
            min_distance = distance;
            nearest_target = {bx, by, Target::BUILDING, building.get()};
        }
    }

    for (auto& tower : field->GetTowers()) { 
        if (tower->IsDestroyed()) continue;  
        int tx = tower->GetX(), ty = tower->GetY();
        int dx = std::abs(tx - x_), dy = std::abs(ty - y_);
        int distance = dx + dy;
        if (distance < min_distance) {
            min_distance = distance;
            nearest_target = {tx, ty, Target::TOWER, tower.get()};
        }
    }

    if (nearest_target.x != -1) {
        int tx = nearest_target.x, ty = nearest_target.y;
        int dx = (tx > x_) ? 1 : (tx < x_) ? -1 : 0;
        int dy = (ty > y_) ? 1 : (ty < y_) ? -1 : 0;
        int nx = x_ + dx, ny = y_ + dy;

        if (min_distance <= 1) {
            if (nearest_target.type == Target::ENEMY) {
                Enemy* enemy = static_cast<Enemy*>(nearest_target.ptr);
                enemy->TakeDamage(GetDamage());
                std::cout << "Ally at (" << x_ << ", " << y_ << ") attacked enemy at (" << tx << ", " << ty << ") for " << GetDamage() << " damage\n";
                if (!enemy->IsAlive()) {
                    field->RemoveEnemy(enemy);
                    if (player) player->AddPoints(10);
                }
            } else if (nearest_target.type == Target::BUILDING) {
                Building* building = static_cast<Building*>(nearest_target.ptr);
                building->TakeDamage(GetDamage());
                std::cout << "Ally at (" << x_ << ", " << y_ << ") attacked building at (" << tx << ", " << ty << ") for " << GetDamage() << " damage\n";
                if (!building->IsAlive()) {
                    field->RemoveBuilding(building);
                }
            } else if (nearest_target.type == Target::TOWER) {
                EnemyTower* tower = static_cast<EnemyTower*>(nearest_target.ptr);
                tower->TakeDamage(GetDamage());
                std::cout << "Ally at (" << x_ << ", " << y_ << ") attacked tower at (" << tx << ", " << ty << ") for " << GetDamage() << " damage\n";
                if (tower->IsDestroyed()) {
                    field->RemoveTower(tower);
                }
            }
        } else {
            if (nx >= 0 && nx < field->GetWidth() && ny >= 0 && ny < field->GetHeight() &&
                field->GetGrid()[ny][nx].GetType() != CellType::kObstacle &&
                field->GetGrid()[ny][nx].GetType() != CellType::kPlayer &&
                field->GetGrid()[ny][nx].GetType() != CellType::kEnemy &&
                field->GetGrid()[ny][nx].GetType() != CellType::kBuilding &&
                field->GetGrid()[ny][nx].GetType() != CellType::kTrap &&
                field->GetGrid()[ny][nx].GetType() != CellType::kTower &&
                field->GetGrid()[ny][nx].GetType() != CellType::kAlly) {
                
                field->GetGrid()[y_][x_].SetType(under_type_);
                under_type_ = field->GetGrid()[ny][nx].GetType();
                SetPosition(nx, ny);
                field->GetGrid()[ny][nx].SetType(CellType::kAlly);
            }
        }
    }
}
