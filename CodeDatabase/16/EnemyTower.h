#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

#include "Entity.h"
#include "Spells.h"
#include <memory>

class EnemyTower : public Entity {
public:
    EnemyTower(int x, int y, int health, int damage, int range, int cooldown);
    void update(Game& game) override;
    int getRange() const;

    void save(std::ostream& os) const override;
    void load(std::istream& is) override;

private:
    int attack_cooldown_;
    int current_cooldown_ = 0;
    std::unique_ptr<ISpell> spell_;
    int range_ = 0;
};

#endif