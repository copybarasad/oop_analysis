#include "objects/Player.h"
#include "objects/Hand.h"

bool Player::castSpell(int spellIndex, Field& field, int targetX, int targetY) {
    return hand.useSpell(spellIndex, field, this, targetX, targetY);
}

void Player::attack(Entity* target) {
    if (target && target->isAlive()) {
        switch (attackMode) {
            default:
            case AttackMode::CLOSE_COMBAT:
                target->takeDamage(damage);
                break;
            case AttackMode::RANGED_COMBAT:
                target->takeDamage(rangeDamage);
                break;
        }
    }
}

void Player::toggleAttackMode() {
    if (attackMode == CLOSE_COMBAT) {
        attackMode = RANGED_COMBAT;
    } else {
        attackMode = CLOSE_COMBAT;
    }
}

std::unique_ptr<GameObject> Player::clone() const {
    auto p = std::make_unique<Player>(x, y);
    p->health = this->health;
    p->maxHealth = this->maxHealth;
    p->damage = this->damage;
    p->speed = this->speed;
    p->attackMode = this->attackMode;
    p->rangeDamage = this->rangeDamage;
    p->range = this->range;
    p->canMove = this->canMove;
    return p;
}
