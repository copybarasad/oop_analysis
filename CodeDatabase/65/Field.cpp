#include "Field.h"
#include "objects/Trap.h"

GameObject* Field::getObjectAt(int x, int y) {
    for (const auto& obj : objects) {
        if (obj->getX() == x && obj->getY() == y) {
            return obj.get();
        }
    }
    return nullptr;
}

void Field::removeDeadEnemies() {
    int xpGainded = 0;

    for (int i = objects.size() - 1; i >= 0; --i) {
        auto& obj = objects[i];

        if (obj->hasTag(ObjectTag::TAG_ENEMY) || obj->hasTag(ObjectTag::TAG_TOWER)) {
            Enemy* enemy = static_cast<Enemy*>(obj.get());
            if (!enemy->isAlive()) {
                xpGainded += player->getXPPerKill();
                objects.erase(objects.begin() + i);
            }
        }

        else if (obj->hasTag(ObjectTag::TAG_TRAP)) {
            Trap* trap = static_cast<Trap*>(obj.get());
            if (trap->isActivated()) {
                objects.erase(objects.begin() + i);
            }
        }
    }

    if (xpGainded > 0 && player) {
        player->addXP(xpGainded);
    }
}

bool Field::hasEnemies() const {
    for (int i = 0; i < objects.size(); i++) {
        if (objects[i]->hasTag(ObjectTag::TAG_ENEMY) || objects[i]->hasTag(ObjectTag::TAG_TOWER)) {
            return true;
        }
    }
    return false;
}
