#include "Enemy.h"
#include "Player.h"
#include "Gamefield.h"
#include <iostream>

Enemy::Enemy(int hp, int damage, int x, int y) {
    this->hp = hp;
    this->damage = damage;
    this->x_pos = x;
    this->y_pos = y;
}

void Enemy::go_to(Cell& cell) {
    Entity::go_to(cell);
    cell.set_enemy(this);
}

void Enemy::move_towards_player(Player& player, Gamefield& field) {
    int dx = player.x_pos - this->x_pos;
    int dy = player.y_pos - this->y_pos;
    int move_x = this->x_pos;
    int move_y = this->y_pos;

    if (std::abs(dx) >= std::abs(dy)) {
        move_x += (dx > 0) ? 1 : (dx < 0) ? -1 : 0;
    } else {
        move_y += (dy > 0) ? 1 : (dy < 0) ? -1 : 0;
    }

    Cell* next_cell = field.get_cell(move_x, move_y);
    if (next_cell && next_cell->is_passable() && !next_cell->is_occupied()) {
        go_to(*next_cell);
    } else if (std::abs(player.x_pos - move_x) <= 1 && std::abs(player.y_pos - move_y) <= 1) {
        player.take_damage(this->damage);
    }

    if(next_cell->is_trap()){
        int dmg = next_cell->get_trap_damage();
        std::cout << "Trap! Enemy gets " << dmg << " damage.\n";
        this->take_damage(dmg);
        if (this->hp <= 0) {
            std::cout << "Enemy defeated! (+10 points!)\n";
            player.edit_score(score_defeat);
            next_cell->set_enemy(nullptr);
        }
    }
}

int Enemy::get_damage() const { return damage; }
int Enemy::get_hp() const { return hp; }
void Enemy::take_damage(int dmg) { hp -= dmg; }