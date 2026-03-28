#include "Enemy.h"


Enemy::Enemy(GameConfig &cfg) : Diver(cfg) {
    hp = cfg.en_hp;
    damage = cfg.en_damage;
}