#include "trap.hpp"
#include "GameBoard.hpp"

Trap::Trap(int x, int y, int trap_damage)
    : Entity(x, y), damage(trap_damage) {}

bool Trap::isTrap() const {
    return true;
}

int Trap::getDamage() const {
    return damage;
}

void Trap::removeFromBoard(GameBoard& board) {
    board.removeTrap(this);
}

void Trap::onSteppedOn(Entity* activator) {
    activator->takeDamage(this->damage);
}
