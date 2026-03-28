#include "enemy_tower.hpp"
#include "zap_spell.hpp"

const int ENEMY_TOWER_HEALTH = 100;
const int ENEMY_TOWER_COOLDOWN = 10;

EnemyTower::EnemyTower(): 
    Building(
        std::make_shared<ZapSpell>(7), 
        ENEMY_TOWER_HEALTH,
        ENEMY_TOWER_COOLDOWN
    ) 
{
    screen_symbol = 'T';
}

EnemyTower::EnemyTower(BuildingImage img): 
    Building(
        std::make_shared<ZapSpell>(7),
        img.health_points,
        img.cooldown,
        img.timer
    ) 
{
    screen_symbol = 'T';
}

BuildingImage EnemyTower::get_data_image() {
    return BuildingImage(
        health_points,
        cooldown,
        timer,
        get_effects_img()
    );
}

Move EnemyTower::choose_move(const std::vector<Position> &step_moves, const std::vector<Position> &attack_moves, const Position &cur_pos, const Position &nearest) {
    timer++;

    if (is_move_available()) {
        return { cur_pos, nearest, MoveType::SPELL, 0 };
    }

    return { cur_pos, NO_POSITION, MoveType::SPELL };
}

GameObject *EnemyTower::clone() const {
    return new EnemyTower(*this);
}