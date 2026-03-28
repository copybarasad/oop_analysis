#include "enemy_tower.h"
#include "direct_damage_spell.h"
#include "player.h"

#include <iostream>


EnemyTower::EnemyTower(std::string name, const Position& position, int health, int range, int damage, int cooldown_turns) :
    Entity(std::move(name), position, health, damage),
    attack_range(range),
    cooldown(cooldown_turns),
    max_cooldown(cooldown_turns){}



bool EnemyTower::can_attack_player(const Player& player) const {
    if (cooldown > 0 || !is_alive()) return false;
    int distance = player.get_position().find_distance_to(position);
    return distance <= attack_range;
}


void EnemyTower::attack_player(Player& player, GameState& game_state) {

    DirectDamageSpell tower_spell("Магический снаряд башни", damage, attack_range, AttackDirection::TOWER_TO_PLAYER);

    std::cout << "Вражеская башня применяет ослабленное заклинание прямого урона!\n";
    tower_spell.cast(player, game_state);     // башня бьет игрока, (2 случай, так как установили значение TOWER_TO_PLAYER)

    cooldown = max_cooldown;
}


void EnemyTower::decrease_cooldown() {
    if (cooldown > 0) cooldown--;
}


int EnemyTower::get_attack_range() const {
    return attack_range;
}


int EnemyTower::get_attack_cooldown() const {
    return cooldown;
}

int EnemyTower::get_max_cooldown() const {
    return max_cooldown;
}





