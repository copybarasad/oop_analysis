#include "Tower.hpp"

Tower::Tower (size_t id, unsigned int maxHealthValue, unsigned int currentHealthValue,
    unsigned int spellCoolDown, unsigned int currentCoolDown, size_t range, unsigned int damageValue) :
    Entity(id, maxHealthValue, currentHealthValue, damageValue), spellCoolDown(spellCoolDown), currentCoolDown(currentCoolDown), range(range) {}

size_t Tower::getRange() const { return range; }

unsigned int Tower::getCurrentCoolDown() const { return currentCoolDown; }

unsigned int Tower::getSpellCoolDown () const { return currentCoolDown; }

void Tower::subCoolDown () { currentCoolDown -= 1; }

std::pair<bool, size_t> Tower::attack (Field& field, size_t casterX, size_t casterY) {
    int targetX = casterX - range, targetY = casterY - range;
    for (int y = 0; y <= (int)range; y++) {
        for (int x = 0; x <= (int)range; x++) {
            if (checkPosition(field, targetX + x, targetY + y)) {
                currentCoolDown = spellCoolDown;
                return std::make_pair(true, field.getCell(targetX+x, targetY + y).getEntity().getID());
            }
        }
    }
    return std::make_pair(false, 0);
}

unsigned int Tower::calculateDamage () const { return 0; }

bool Tower::checkPosition (Field& field, int targetX, int targetY) const {
    return  targetX >= 0 && targetX <= (int)field.getWidth() && targetY >= 0 && targetY <= (int)field.getHeight()
        && !field.getCell(targetX, targetY).isEmpty() && field.getCell(targetX, targetY).isPlayer();
}