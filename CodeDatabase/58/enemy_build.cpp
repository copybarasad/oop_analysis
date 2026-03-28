#include "enemy_build.hpp"


EnemyBuild::EnemyBuild(int required_quantity, int hp, int damage) : Entity(hp, damage), required_quantity(required_quantity) {};
EnemyBuild::EnemyBuild(EnemyBuild* enemy_build) : Entity(enemy_build->get_hp(), enemy_build->get_damage()), required_quantity(enemy_build->get_required_quantity()) {};

int EnemyBuild::get_required_quantity() {return required_quantity;};

char EnemyBuild::get_symbol() {return 'B';};