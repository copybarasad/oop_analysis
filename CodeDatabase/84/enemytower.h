#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

#include "entity.h"
#include "directdamagespell.h"

class Player;
class Field;

class EnemyTower : public Entity {
private:
    int attack_range_;
    std::unique_ptr<DirectDamageSpell> spell_;
    bool can_attack_;
    int cooldown_timer_;
    static const int ATTACK_COOLDOWN = 2;

public:
    EnemyTower(int health, int x, int y, int attack_range, int attack_damage);
    
    bool can_attack() const;
    void set_can_attack(bool can_attack);
    int get_attack_range() const;
    int get_damage() const;
    bool try_attack_player(Field& field, const Player* player);
    void update();

    void save(std::ostream& os) const override;
    void load(std::istream& is) override;
};

#endif