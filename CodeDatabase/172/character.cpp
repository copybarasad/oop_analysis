#include "character.h"

namespace game {
    // ---------- Calculate new HP after damage -----------
    void Character::takeDamage(int dmg)
    {
        if (dmg > 0) {
            health_ -= dmg;
            
            if (health_ < 0) {
                health_ = 0;
            }
        }
    }
}