#include <PlayerTrap.hpp>
#include <Constants.hpp>

PlayerTrap::PlayerTrap(int x, int y) 
    : Entity(x, y, Entity::Type::PLAYER_TRAP), damage(PLAYER_TRAP_DAMAGE) {}

void PlayerTrap::draw() {
    mvaddch(y, x, sprite);
}

int PlayerTrap::getDamage() const {
    return damage;
}

void PlayerTrap::empower(int charges) {
    damage += charges * PLAYER_TRAP_EMPOWER_BONUS;
}

void PlayerTrap::setDamage(int value) {
    this->damage = value;
}