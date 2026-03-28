#ifndef INCLUDE_FRIEND_ENEMY
#define INCLUDE_FRIEND_ENEMY

#include "enemy.h"

class FriendEnemy : public Enemy {
public:
    FriendEnemy();
    FriendEnemy(int max_health);
    
    bool is_friendly() const { return true; }
};

#endif