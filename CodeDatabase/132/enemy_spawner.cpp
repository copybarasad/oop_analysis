#include "enemy_spawner.hpp"
#include "spawn_enemy_spell.hpp"

const int ENEMY_SPAWNER_HEALTH = 100;
const int ENEMY_SPAWNER_COOLDOWN = 10;

EnemySpawner::EnemySpawner(): 
    Building(
        std::make_shared<SpawnEnemySpell>(),
        ENEMY_SPAWNER_HEALTH,
        ENEMY_SPAWNER_COOLDOWN
    ) 
{
    screen_symbol = 'B';
}

// TODO:
EnemySpawner::EnemySpawner(int speed): 
    Building(std::make_shared<SpawnEnemySpell>(), 100, speed) 
{
    screen_symbol = 'B';
}

EnemySpawner::EnemySpawner(BuildingImage img): 
    Building(
        std::make_shared<SpawnEnemySpell>(),
        img.health_points,
        img.cooldown,
        img.timer
    ) 
{
    screen_symbol = 'B';
}

BuildingImage EnemySpawner::get_data_image() {
    return BuildingImage(
        health_points,
        cooldown,
        timer,
        get_effects_img()
    );
}

Move EnemySpawner::choose_move(const std::vector<Position> &step_moves, const std::vector<Position> &attack_moves, const Position &cur_pos, const Position &nearest) {
    timer++;

    std::random_device rd;
    std::mt19937 gen(rd());

    if (step_moves.size() != 0 && is_move_available()) {
        return { cur_pos, step_moves[gen() % step_moves.size()], MoveType::SPELL, 0 };
    }
    
    printf("no available moves for B (%d %d)\n", cur_pos.x, cur_pos.y);
    return { cur_pos, NO_POSITION, MoveType::SPELL };
}

GameObject *EnemySpawner::clone() const {
    return new EnemySpawner(*this);
}