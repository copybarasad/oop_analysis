#include "EnemyTower.h"
#include "Game.h"
#include <cmath>
#include <iostream>

EnemyTower::EnemyTower(int x, int y, int health, int damage, int range, int cooldown)
    : Entity(x, y, health, damage), 
      attack_cooldown_(cooldown),
      range_(range) {
      spell_ = std::make_unique<DirectDamageSpell>(damage, range);
}

int EnemyTower::getRange() const {
    return range_;
}

void EnemyTower::update(Game& game) {
    if (!isAlive()) return;

    if (current_cooldown_ > 0) {
        current_cooldown_--;
        return;
    }

    Player* player = game.getEntityManager().getPlayer();
    if (player) {
        if (spell_->cast(game, *this, player->getPosition())) {
             current_cooldown_ = attack_cooldown_;
        }
    }
}

void EnemyTower::save(std::ostream& os) const {
    Entity::save(os);
    os << attack_cooldown_ << " " << current_cooldown_ << " " << range_ << " ";
}

void EnemyTower::load(std::istream& is) {
    Entity::load(is);
    is >> attack_cooldown_ >> current_cooldown_ >> range_;
    spell_ = std::make_unique<DirectDamageSpell>(damage_, range_);
}