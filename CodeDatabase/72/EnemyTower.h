//
// Created by Mac on 26.10.2025.
//

#ifndef GAME_TERM_ENEMYTOWER_H
#define GAME_TERM_ENEMYTOWER_H
#include "Character.h"
#include "Entity.h"
#include "../utils /ISerializable.h"

#define DEFAULT_COOLDOWN 3
#define DEFAULT_RADIUS_TOWER 3
#define DEFAULT_HEALTH_TOWER 200
#define DEFAULT_DAMAGE_TOWER 15


class Field;

class EnemyTower: public Character, public ISerializable{

    int radius;
    bool canAttack;
    int cooldown;

public:

    explicit EnemyTower(const Position & position = {0, 0}, const int & health = DEFAULT_HEALTH_TOWER, const int & damage = DEFAULT_DAMAGE_TOWER,
        const int & cooldown = DEFAULT_COOLDOWN, const int & radius = DEFAULT_RADIUS_TOWER);

    void attack(Field & field);

    void save(Saver &saver) const override;

    void load(Reader &reader) override;
};


#endif //GAME_TERM_ENEMYTOWER_H