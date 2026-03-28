//
// Created by Mac on 26.10.2025.
//

#include "TrapSpell.h"

#include <iostream>
#include <unistd.h>

#include "../TargetSelection.h"

TrapSpell::TrapSpell(const int & damage)
    :damage(damage) {

    if (damage < 0)
        this->damage = DEFAULT_TRAP_DAMAGE;
}

Type TrapSpell::getType() const {
    return Type::Trap;
}

std::string TrapSpell::getName() const {
    return "Trap spell";
}

bool TrapSpell::canPost(Field &field, const Position &target) const {
    if (field.getCell(target).getState() != State::Free) {
        return false;
    }
    auto towers = field.getEnemyTowers();
    for (auto tower: towers) {
        if (target == tower.getPosition()) {
            return false;
        }
    }
    auto enemyBuilds = field.getEnemiesBuild();
    for (auto build: enemyBuilds) {
        if (target == build.getPosition()) {
            return false;
        }
    }
    return true;
}

void TrapSpell::cast(Field &field) {

    TargetSelection selection(field);

    Position position = selection.selectArea(1, 1, 10);

    if (position == Position{-1,-1} || !canPost(field, position)) {
        if (position != Position{-1, -1})
            std::cout << "You cannot set a trap in this place." << std::endl;
        sleep(2);
        field.getPlayer()->getHand()->addSpell(std::make_shared<TrapSpell>(*this));
        return;
    }

    field.addTrap(Trap(position, damage));
}

void TrapSpell::save(Saver &saver) const {
    saver.save(damage);
}

void TrapSpell::load(Reader &reader) {
    reader.read(damage);
}




