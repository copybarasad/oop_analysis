#include "Player.h"
#include "Enemy.h"
#include "Hand.h"

Player::Player(int hp, int x, int y, size_t hand_capacity) {
    this->hp = hp;
    this->x_pos = x;
    this->y_pos = y;
    current_weapon = Weapon::Melee;
    damage = melee_damage;
    max_hp = hp;
    score = 0;
    hand = std::make_unique<Hand>(hand_capacity);
    hand->add_random_spell();
}

void Player::go_to(Cell& cell) {
    if (can_move && cell.is_passable() && !cell.is_occupied()){
        Entity::go_to(cell);
        current_cell->set_player(nullptr);
        cell.set_player(this);
        if (cell.is_slow()) slowed = true;
    } else if (cell.is_occupied()){
        Enemy* enemy = cell.get_enemy();
        if (enemy != nullptr) {
            std::cout << "Player attacks the enemy!\n";
            enemy->take_damage(damage);

            if (enemy->hp <= 0) {
                std::cout << "Enemy defeated! (+10 points!)\n";
                this->score += 10;
                cell.set_enemy(nullptr);
            }
        }

    }
}

void Player::switch_weapon() {
    if (current_weapon == Weapon::Melee){
        current_weapon = Weapon::Range;
        damage = range_damage;
    } else {
        current_weapon = Weapon::Melee;
        damage = melee_damage;
    }
}

void Player::start_turn() {
    if (slowed) {
        can_move = false;
        slowed = false;
        std::cout << "Player is slowed and skips the turn\n";
    } else {
        can_move = true;
    }
}

int Player::get_score() const { return score; }
int Player::get_damage() const { return damage; }
void Player::edit_score(int value) { score = score + value; }
void Player::take_damage(int dmg) { hp -= dmg; }
int Player::get_x() const { return x_pos; }
int Player::get_y() const { return y_pos; }
void Player::edit_max_hp(int value){
    max_hp += value;
    hp += value;
}
void Player::set_max_hp(int v) {
    max_hp = v;
    if (hp > max_hp) hp = max_hp;
}
void Player::heal(int amount){
    hp += amount;
    if (hp > max_hp) hp = max_hp;
}
void Player::reset_hand(size_t cap) {
    hand = std::make_unique<Hand>(cap);
}