//
// Created by Mac on 25.10.2025.
//

#include "DirectDamageSpell.h"
#include "ISpell.h"

#include <iostream>
#include <unistd.h>

#include "../TargetSelection.h"


DirectDamageSpell::DirectDamageSpell(const int &  damage, const int &radius, const int & areaSize)
    :damage(damage), radius(radius) {

    if (damage <= 0)
        this->damage = DEFAULT_DAMAGE_DIRECT;

    if (radius <= 0)
        this->radius = DEFAULT_RADIUS_DIRECT;

    if (areaSize <= 0)
        this->areaSize = DEFAULT_AREA_SIZE_DIRECT;
}

std::string DirectDamageSpell::getName() const {
    return "Direct damage";
}


void DirectDamageSpell::cast(Field & field) {

    TargetSelection selection(field);

    Position target = selection.selectArea(1, 1, radius);

    if (target != Position{-1, -1}) {
        auto & enemies = field.getEnemiesMutable();

        auto & enemy_builds = field.getEnemiesBuildMutable();

        auto & towers = field.getEnemyTowersMutable();

        for (auto it = enemies.begin(); it != enemies.end(); ++it) {
            if (it->getPosition() == target) {
                it->applyDamage(damage);
                if (!it->isAlive()) {
                    it = enemies.erase(it);
                }
                return;
            }
        }

        for (auto it = enemy_builds.begin(); it != enemy_builds.end(); ++it) {
            if (it->getPosition() == target) {
                it->applyDamage(damage);
                if (!it->isAlive()) {
                    it = enemy_builds.erase(it);
                }
                return;
            }
        }

        for (auto it = towers.begin(); it != towers.end(); ++it) {
            if (it->getPosition() == target) {
                it->applyDamage(damage);
                if (!it->isAlive()) {
                    it = towers.erase(it);
                }
                return;
            }
        }
        std::cout << "No enemy was chosen." << std::endl;
    }else if (target != Position{-1, -1}) {
        std::cout << "The spell can only be cast within a radius of 3 cells." << std::endl;
    }

    sleep(2);
    field.getPlayer()->getHand()->addSpell(std::make_shared<DirectDamageSpell>(*this));
}

void DirectDamageSpell::castOnPlayer(std::shared_ptr<Player> player) {
    player->applyDamage(damage);
}

Type DirectDamageSpell::getType() const {
    return Type::DirectDamage;
}

void DirectDamageSpell::save(Saver &saver) const {
    saver.save(damage);
    saver.save(radius);
    saver.save(areaSize);
}

void DirectDamageSpell::load(Reader &reader) {
    reader.read(damage);
    reader.read(radius);
    reader.read(areaSize);
}




