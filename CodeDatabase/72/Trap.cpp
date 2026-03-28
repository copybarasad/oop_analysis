//
// Created by Mac on 26.10.2025.
//

#include "Trap.h"

Trap::Trap(const Position &position, const int &damage)
    :Entity(position), damage(damage), active(true){
    if (damage < 0)
        this->damage = DEFAULT_DAMAGE_TRAP;
}

int Trap::getDamage() const { return this->damage ;}

bool Trap::isActive() const { return this->active; }

void Trap::deactivate() { this->active = false;}

void Trap::load(Reader &reader) {
    reader.read(damage);
    reader.read(active);
    position.load(reader);
}

void Trap::save(Saver &saver) const {
    saver.save(damage);
    saver.save(active);
    position.save(saver);
}




