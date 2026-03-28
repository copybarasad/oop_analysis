//
// Created by bob on 10/4/25.
//

#ifndef GAME_TERM_ENEMY_H
#define GAME_TERM_ENEMY_H
#include "Character.h"
#include "../utils /ISerializable.h"


#define DEFAULT_DAMAGE 20
#define DEFAULT_HEALTH 100
#define DEFAULT_POSITION {0, 0}


class Enemy: public Character, public ISerializable{

public:

    explicit Enemy(const int & damage = DEFAULT_DAMAGE, const Position & position = DEFAULT_POSITION, const int & health = DEFAULT_HEALTH);

    void save(Saver &saver) const override;

    void load(Reader &reader) override;
};


#endif //GAME_TERM_ENEMY_H