#include "enemy.h"
#include "player.h"        
#include "game_field.h"   
#include "pathfinder.h"
#include <cmath>
#include <iostream>
#include <stdexcept>

Enemy::Enemy(Position start, int hp, int damage)
    : position_(start), health_(hp), damage_(damage) {}

Enemy::Enemy(Position start) 
    : Enemy(start, 50, 10) {}

Position Enemy::GetPosition() const { return position_; }
void Enemy::SetPosition(const Position& pos) { position_ = pos; }

void Enemy::TakeDamage(int dmg) {
	if (dmg < 0) throw std::invalid_argument("Урон не может быть отрицательным");
	health_ = std::max(0, health_ - dmg);
}

bool Enemy::IsAlive() const { return health_ > 0; }
int Enemy::GetDamage() const { return damage_; }
int Enemy::GetHealth() const { return health_; }

void Enemy::MoveTowards(Player& player, GameField& field, const std::vector<Enemy>& others) {
    if (!IsAlive()) return;

    Position start = position_;
    Position goal = player.GetPosition();

    if (field.HasTrap(position_)) {
        int trapDamage = field.TriggerTrap(position_);
        TakeDamage(trapDamage);
        std::cout << "Враг наступил на ловушку и получил " << trapDamage << " урона!\n";
        
        if (!IsAlive()) {
            std::cout << "Враг уничтожен ловушкой! +10 очков\n";
            player.AddScore(10);
        }
        return; 
    }

    if (std::abs(goal.x - start.x) + std::abs(goal.y - start.y) == 1) {
        player.TakeDamage(damage_);
        std::cout << "Враг атакует и наносит " << damage_ << " урона!\n";
        return;
    }

    Position step;
    if (Pathfinder::FindStep(field, start, goal, step, others)) {
        if (field.HasTrap(step)) {
            int trapDamage = field.TriggerTrap(step);
            TakeDamage(trapDamage);
            std::cout << "Враг наступил на ловушку и получил " << trapDamage << " урона!\n";
            
            if (!IsAlive()) {
                std::cout << "Враг уничтожен ловушкой! +10 очков\n";
                player.AddScore(10);
            }
            position_ = step;
        } else {
            if (step != player.GetPosition()) position_ = step;
        }
    }
}
