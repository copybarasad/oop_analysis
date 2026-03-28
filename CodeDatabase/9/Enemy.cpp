#include "Enemy.h"
#include "Config.h"


Enemy::Enemy(int hp, int damage, std::pair<int, int> pos):Character(ENEMY_HP, ENEMY_DAMAGE, pos){
    skipNextTurn_ = false;
}

void Enemy::step(std::pair<int, int> delta){
    moveBy(delta);
}

bool Enemy::shouldSkipTurn(){
    return skipNextTurn_;
}

void Enemy::setSkipNextTurn(bool v){
    skipNextTurn_ = v;
}