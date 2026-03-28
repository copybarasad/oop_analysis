//
// Created by Mac on 26.10.2025.
//

#ifndef GAME_TERM_TRAP_H
#define GAME_TERM_TRAP_H
#include "Entity.h"
#include "../Position.h"
#include "../utils /ISerializable.h"


#define DEFAULT_DAMAGE_TRAP 40

class Trap: public Entity, public ISerializable{
    int damage;
    bool active;

public:
    explicit Trap(const Position & position = {0, 0}, const int & damage = DEFAULT_DAMAGE_TRAP);

    int getDamage() const;

    bool isActive() const;

    void deactivate();

    void load(Reader &reader) override;

    void save(Saver &saver) const override;
};


#endif //GAME_TERM_TRAP_H