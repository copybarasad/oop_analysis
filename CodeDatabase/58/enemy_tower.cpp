#include "enemy_tower.hpp"

EnemyTower::EnemyTower(int hp, int damage) : Entity(hp, damage) {};
EnemyTower::EnemyTower(int hp, int damage, int elapsed_since_shot) : EnemyTower(hp, damage) {this->elapsed_since_shot = elapsed_since_shot;};
EnemyTower::EnemyTower(EnemyTower* et) : EnemyTower(et->get_hp(), et->get_base_damage()) {};

WaterBall& EnemyTower::get_waterball() {return waterball;};

int EnemyTower::get_range() {return waterball.get_range();};

int EnemyTower::get_elapsed_since_shot() {return elapsed_since_shot;};
void EnemyTower::add_elapsed_since_shot() {elapsed_since_shot++;};
void EnemyTower::set_elapsed_since_shot(int n) {elapsed_since_shot = n;};

char EnemyTower::get_symbol() {return 'T';};