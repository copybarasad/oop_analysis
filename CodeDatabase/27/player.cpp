#include "player.h"
#include "enemy.h"
#include "hand.h"

Player::~Player() = default;

Player::Player(size_t hand_capacity) 
    : Unit(nullptr, DEFAULT_HP, DEFAULT_DAMAGE, true, DEFAULT_X, DEFAULT_Y),
      max_hp(DEFAULT_HP)
{
    current_disk = disk::melee;
    score = 0;
    damage = MELEE_DAMAGE;
    hand = std::make_unique<Hand>(hand_capacity);
    hand->add_random_spell();
}

void Player::on_move_to_point(Point& point) {
    if (point.is_occupied() && point.get_occupier() != nullptr) {
        if (Enemy* enemy = dynamic_cast<Enemy*>(point.get_occupier())) {
            enemy->take_damage(this->damage); 
            
            if (enemy->get_hp() <= 0) {
                score += 10;
                point.set_occupier(nullptr); 
            }
        }
    } else {
        point.set_occupier(this);
    }
}

void Player::switch_weapon() {
    if (current_disk == disk::melee) {
        current_disk = disk::range;
        damage = RANGE_DAMAGE;
    } else {
        current_disk = disk::melee;
        damage = MELEE_DAMAGE;
    }
}

void Player::set_disk_type(int v) {
    if (v == static_cast<int>(disk::melee)) {
        current_disk = disk::melee;
        damage = MELEE_DAMAGE; 
    } else {
        current_disk = disk::range;
        damage = RANGE_DAMAGE; 
    }
}

void Player::increase_damage(int delta) {
    damage += delta;
}

void Player::increase_max_hp(int amt) {
    max_hp += amt;
    hp += amt;
}

void Player::heal(int amount) {
    hp += amount;
    if (hp > max_hp) hp = max_hp;
}

void Player::set_hp(int v) {
    hp = std::max(0, std::min(v, max_hp));
}

