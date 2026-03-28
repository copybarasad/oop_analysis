#include "Enemy.h"



Enemy::Enemy(double healthEnemy, double damageEnemy, int pos_xEnemy, int pos_yEnemy)
    : GameEnt(healthEnemy, damageEnemy, pos_xEnemy, pos_yEnemy, GameEntType::Enemy){}

