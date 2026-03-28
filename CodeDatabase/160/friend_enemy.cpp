#include "../include/entity/friend_enemy.h"

FriendEnemy::FriendEnemy() : Enemy(50, 0) {
}

FriendEnemy::FriendEnemy(int max_health) : Enemy(max_health, 0) {
}