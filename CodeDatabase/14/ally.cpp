#include "ally.hpp"
#include "GameBoard.hpp"

Ally::Ally(int x, int y, int start_health, int ally_damage)
    : Entity(x, y), health(start_health), damage(ally_damage) {}

int Ally::getHealth() const {
    return health;
}

int Ally::getDamage() const {
    return damage;
}

void Ally::takeDamage(int dmg) {
    health -= dmg;
}

bool Ally::isAlive() const {
    return health > 0;
}

void Ally::removeFromBoard(GameBoard& board) {
    board.removeAlly(this);
}
