#include "enemy.h"
#include "player.h"

Enemy::Enemy(): maxhealth(100), health(100), damage(15) {}

int Enemy::GetHealth() const {
    return health;
}

int Enemy::GetDamage() const {
    return damage;
}
void Enemy::Damage(int PlayerDanage){
    health -= PlayerDanage;
    if (health < 0) health = 0;
}

bool Enemy::IsLife() const {
    return health > 0;
}

bool Enemy::EnemyAttack(Player& player){
    player.Damage(GetDamage());
    if (!player.IsLife()){
        return true;
    } else {
        return false;
    }
}