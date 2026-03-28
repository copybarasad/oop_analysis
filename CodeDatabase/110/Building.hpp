#ifndef BUILDING_H
#define BUILDING_H

#include "../../Entity/Entity.hpp"
#include "../../Enemies/Enemy.hpp"
#include "../../../Field/Field/Field.hpp"

class Building : public Entity {
    private:
    size_t spawnCoolDown;
    size_t currentCoolDown;
    std::pair<unsigned int, unsigned int> enemyMinStats;

    public:
    Building (size_t id, unsigned int maxHealthValue, unsigned int currentHealthValue, size_t spawnCoolDownValue, size_t currentCoolDownValue,
        unsigned int damageValue=0, std::pair<unsigned int, unsigned int> enemyMinStats={10, 5});

    size_t getMaxCoolDown () const;
    size_t getSpawnCoolDown () const;
    std::pair<unsigned int, unsigned int> getEnemyMinStats() const;

    Enemy* spawnEnemy (size_t id);
    unsigned int calculateDamage () const;
    void subCoolDown ();

    private:
    unsigned int generateEnemyHealth (unsigned int enemyMinHealth) const;
    unsigned int generateEnemyDamage (unsigned int enemyMinDamage) const;
};

#endif