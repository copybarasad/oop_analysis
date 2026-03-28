//
// Created by Mac on 25.10.2025.
//

#include "AreaDamageSpell.h"
#include "../TargetSelection.h"

AreaDamageSpell::AreaDamageSpell(const int & damage,const int & areaSize, const int & radius)
    :damage(damage), areaSize(areaSize), radius(radius){
    if (damage <= 0)
        this->damage = DEFAULT_DAMAGE_AREA;

    if (areaSize <= 0)
        this->areaSize = DEFAULT_AREA_SIZE;

    if (radius <= 0)
        this->radius = DEFAULT_RADIUS_AREA;
}

std::string AreaDamageSpell::getName() const {
    return "Area damage";
}

void AreaDamageSpell::cast(Field & field) {
    TargetSelection selection(field);

    Position target = selection.selectArea(areaSize, areaSize, 3);

    auto & enemies = field.getEnemiesMutable();

    auto & enemy_builds =  field.getEnemiesBuildMutable();

    auto & towers = field.getEnemyTowersMutable();

    for (int y = target.getY(); y < target.getY() + areaSize; y++) {
        for (int x = target.getX(); x < target.getX() + areaSize;x++) {
            Position position = {x, y};
            for (auto it = enemies.begin(); it != enemies.end(); ++it) {
                if (it->getPosition() == position) {
                    it->applyDamage(damage);
                    if (!it->isAlive()) {
                        it = enemies.erase(it);
                    }
                }
            }
            for (auto it = enemy_builds.begin(); it != enemy_builds.end(); ++it) {
                if (it->getPosition() == position) {
                    it->applyDamage(damage);
                    if (!it->isAlive()) {
                        it = enemy_builds.erase(it);
                    }
                }
            }

            for (auto it = towers.begin(); it != towers.end(); ++it) {
                if (it->getPosition() == position) {
                    it->applyDamage(damage);
                    if (!it->isAlive()) {
                        it = towers.erase(it);
                    }
                }
            }
        }
    }
}

Type AreaDamageSpell::getType() const {
    return Type::AreaDamage;
}

void AreaDamageSpell::save(Saver &saver) const {
    saver.save(damage);
    saver.save(areaSize);
    saver.save(radius);
}

void AreaDamageSpell::load(Reader &reader) {
    reader.read(damage);
    reader.read(areaSize);
    reader.read(radius);
}


