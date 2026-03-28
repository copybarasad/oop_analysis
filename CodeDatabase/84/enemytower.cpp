#include "enemytower.h"
#include "player.h"
#include "gamefield.h"
#include <cmath>
#include <iostream>

EnemyTower::EnemyTower(int health, int x, int y, int attack_range, int attack_damage)
    : Entity(health, nullptr, x, y), attack_range_(attack_range), 
      can_attack_(true), cooldown_timer_(0) {
    spell_ = std::make_unique<DirectDamageSpell>(
        "Магический выстрел", "Ослабленная магическая атака башни", 
        attack_damage, attack_range, 0);
}

bool EnemyTower::can_attack() const { return can_attack_; }
void EnemyTower::set_can_attack(bool can_attack) { can_attack_ = can_attack; }
int EnemyTower::get_attack_range() const { return attack_range_; }
int EnemyTower::get_damage() const { return spell_->get_damage(); }

bool EnemyTower::try_attack_player(Field& field, const Player* player) {
    if (!player || !player->is_alive() || !can_attack_ || cooldown_timer_ > 0) {
        return false;
    }
    int distance = std::abs(get_x() - player->get_x()) + std::abs(get_y() - player->get_y());
    if (distance <= attack_range_) {
        if (spell_->cast(field, player->get_x(), player->get_y())) {
            can_attack_ = false;
            cooldown_timer_ = ATTACK_COOLDOWN;
            return true;
        }
    }
    return false;
}

void EnemyTower::update() {
    if (cooldown_timer_ > 0) cooldown_timer_--;
    if (cooldown_timer_ == 0) can_attack_ = true;
}

void EnemyTower::save(std::ostream& os) const {
    Entity::save(os);
    os << " " << attack_range_ << " " << can_attack_ << " " << cooldown_timer_ << " " << spell_->get_damage();
}

void EnemyTower::load(std::istream& is) {
    Entity::load(is);
    int dmg;
    is >> attack_range_ >> can_attack_ >> cooldown_timer_ >> dmg;
    spell_ = std::make_unique<DirectDamageSpell>("Магический выстрел", "Восстановлено", dmg, attack_range_, 0);
}