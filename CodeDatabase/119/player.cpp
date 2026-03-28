#include "player.h"
#include "prints.h"

void Player::switchMode() {
    if (mode == MELEE) {
        mode = RANGED;
        dmg = 5;
        Prints::print_changed_on_ranged_string();
    } else {
        mode = MELEE;
        dmg = 10;
        Prints::print_changed_on_melee_string();
    }
}

void Player::rangedAttack(Entity* enemy) {
    
    if (mode == RANGED) {
        if ((getPosX() == enemy->getPosX() && std::abs(getPosY() - enemy->getPosY()) <= 4) ||
            (getPosY() == enemy->getPosY() && std::abs(getPosX() - enemy->getPosX()) <= 4)) {
            enemy->takeDamage(dmg);
            Prints::print_range_attack_string();
            if (!enemy->isAlive()) {
                Prints::print_entity_killed_other(this->getName(), enemy->getName());
            }
        }
        else Prints::print_range_miss_string();
    }
}

void Player::castSpell(size_t spellIndex)
{
    hand.useSpell(spellIndex, this->getPosX(), this->getPosY(), this);
}
