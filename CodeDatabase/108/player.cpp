#include "player.h"
#include <algorithm>
#include <iostream>
#include <utility>

#include "game_state.h"
#include "field.h"


Player::Player(std::string name, const Position& start_position, int hp, int melee_dmg, int range_dmg) :
    Entity(std::move(name), start_position, hp, melee_dmg),
    score(0),
    combat_mode(CombatMode::MELEE),
    melee_damage(melee_dmg),
    ranged_damage(range_dmg),
    melee_range(1),
    ranged_range(2),
    is_slowed(false),
    slow_cooldown(0),

    spell_hand(5) {}


void Player::apply_slow_effect() {
    is_slowed = true;
    slow_cooldown = 1;        // замедление на 1 ход
}

void Player::update_effects() {
    if (is_slowed) {
        slow_cooldown--;
        if (slow_cooldown <= 0) {
            is_slowed = false;
            slow_cooldown = 0;
        }
    }
}


void Player::move(Direction direction, GameState& game_state) {  // улучшено после 1 лб (проверки на корректность хода)

    Position new_position = position;
    switch (direction) {
        case Direction::UP:
            new_position.set_y(position.get_y() + 1);
            break;
        case Direction::DOWN:
            new_position.set_y(position.get_y() - 1);
            break;
        case Direction::LEFT:
            new_position.set_x(position.get_x() - 1);
            break;
        case Direction::RIGHT:
            new_position.set_x(position.get_x() + 1);
    }

    // conditions for movement check!!!
    if (!game_state.get_field().position_is_valid(new_position) ) {
        std::cout << "Нельзя выйти за границы поля!\n";
        return;
    }

    if (!game_state.get_field().position_is_passable(new_position, game_state)) {
        std::cout << "Здесь препятствие! Нельзя пройти.\n";
        return;
    }

    if (is_slowed) {
        std::cout << "Игрок замедлен и не может двигаться 1 ход!\n";
        update_effects();

        return;
    }

    position = new_position;

    Cell current_cell = game_state.get_field().get_cell_at(position);
    if (current_cell.get_type() == CellType::SLOW) {
        apply_slow_effect();
        std::cout << "Вы наступили на замедляющую клетку! Пропуск следующего хода.\n";
    }

}



// fixed
bool Player::attack_nearest_target(GameState &game_state) {

    Entity* nearest_target = game_state.find_nearest_target(position, get_attack_range());

    // Атакуем найденную цель
    if (nearest_target) {
        int initial_health = nearest_target->get_health();
        nearest_target->take_damage(get_current_damage());

        std::string name = nearest_target->get_name();

        std::cout << "Игрок атаковал " << name;

        std::cout << " на (" << nearest_target->get_position().get_x()
                  << "," << nearest_target->get_position().get_y() << ")!\n";
        std::cout << "Урон: " << get_current_damage() << " | Здоровье цели: "
                  << initial_health << " -> " << nearest_target->get_health() << "\n";


        if (!nearest_target->is_alive()) {

            if (name.find("Enemy") != std::string::npos) {
                add_score(10);
                std::cout << "Уничтожен враг! +10 очков\n";
            } else if (name.find("Spawner") != std::string::npos) {
                add_score(20);
                std::cout << "Уничтожен спавнер! +20 очков\n";
            } else if (name.find("Tower") != std::string::npos) {
                add_score(15);
                std::cout << "Уничтожена башня! +15 очков\n";
            } else {
                // на всякий случай
                std::cout << "Уничтожена неизвестная цель! +0 очков\n";
            }
        }

        return true;
    }

    return false;     // никого не атаковал, так как не может

}


bool Player::can_attack_target(const Position& target_position) const {
    return position.find_distance_to(target_position) <= get_attack_range();
}

bool Player::get_is_slowed_info() const {
    return is_slowed;
}


void Player::switch_combat_mode() {
    combat_mode = (combat_mode == CombatMode::MELEE)
                  ? CombatMode::RANGED
                  : CombatMode::MELEE;
}


CombatMode Player::get_combat_mode() const {
    return combat_mode;
}

int Player::get_attack_range() const {
    return (combat_mode == CombatMode::MELEE) ? melee_range : ranged_range;
}

int Player::get_current_damage() const {
    return (combat_mode == CombatMode::MELEE) ? melee_damage : ranged_damage;
}

int Player::get_score() const {
    return score;
}

void Player::add_score(int points) {
    score += points;
}

int Player::get_melee_damage() const {
    return melee_damage;
}

int Player::get_ranged_damage() const {
    return ranged_damage;
}

void Player::set_combat_mode(CombatMode mode) {
    combat_mode = mode;
}

void Player::set_melee_damage(int melee_dmg) {
    melee_damage = melee_dmg;
}

void Player::set_ranged_damage(int ranged_dmg) {
    ranged_damage = ranged_dmg;
}

void Player::set_score(int new_score) {
    score = new_score;
}

int Player::get_melee_range() const {
    return melee_range;
}

int Player::get_ranged_range() const {
    return ranged_range;
}

void Player::set_melee_range(int new_range) {
    melee_range = new_range;
}

void Player::set_ranged_range(int new_range) {
    ranged_range = new_range;
}


// FOR SPELLS INSTEAD (FIXED):
SpellHand& Player::get_spell_hand() {
    return spell_hand;
}

const SpellHand& Player::get_spell_hand() const {
    return spell_hand;
}

