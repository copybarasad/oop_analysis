//
// Created by Mac on 26.10.2025.
//

#include "EnemyTower.h"

#include "../spells/DirectDamageSpell.h"

EnemyTower::EnemyTower(const Position &position, const int & health, const int & damage, const int &cooldown, const int & radius)
    :Character(damage, position, health), cooldown(cooldown), radius(radius) {
    if (cooldown < 0)
        this->cooldown = DEFAULT_COOLDOWN;

    if (radius < 0)
        this->radius = DEFAULT_RADIUS_TOWER;
}

void EnemyTower::attack(Field &field) {

    auto player = field.getPlayer();

    int dx = std::abs(player->getPosition().getX() - this->getPosition().getX());
    int dy = std::abs(player->getPosition().getY() - this->getPosition().getY());
    int distance = std::max(dx, dy);

    if (field.getSteps() % cooldown == 0 && distance <= radius) {
        DirectDamageSpell spell(damage, radius, radius);
        spell.castOnPlayer(player);
    }
}

void EnemyTower::load(Reader &reader) {
    reader.read(damage);
    reader.read(health);
    position.load(reader);
    reader.read(radius);
    reader.read(canAttack);
    reader.read(cooldown);
}

void EnemyTower::save(Saver &saver) const {
    saver.save(damage);
    saver.save(health);
    position.save(saver);
    saver.save(radius);
    saver.save(canAttack);
    saver.save(cooldown);
}


