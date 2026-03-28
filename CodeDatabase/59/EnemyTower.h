#pragma once
#include "Coordinates.h"
#include "DamagingSpell.h"
#include "Being.h"

class EnemyTower {
private:
    DamagingSpell spell;
    int delay;
    int curr_delay = 0;
    Coordinates position;
    float health;
    int maxHealth;
public:
    EnemyTower(int damage, float range, int delay, Coordinates position, int maxHealth) :
    spell(DamagingSpell(BasicSpell::Type::Direct, damage, range, "Заклинание башни")),
    delay(delay), position(position), health(maxHealth), maxHealth(maxHealth) {}

    ~EnemyTower() = default;
    Coordinates getPosition() const { return position; }
    void takeDamage(float dmg) { health -= dmg;
}
    bool can_attack(Coordinates target_pos);
    void tick_delay() { curr_delay = std::max(0, curr_delay - 1);
}
    float attack();
    float getHealth() { return health; }
    int getMaxHealth() { return maxHealth;
}

    void setHealth(float h) { health = h;
}
    void setCurrentDelay(int d) { curr_delay = d; }
    float getRange() { return spell.getRange();
}
    int getDamage() { return spell.getDamage(); }
    int getDelay() { return delay;
}
    int getCurrentDelay() { return curr_delay; }

    json toJson() const;
    void fromJson(const json& j);
};