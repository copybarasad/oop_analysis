#include "Ally.h"
#include "Game.h"

Ally::Ally(int startX, int startY, int allyHealth, int allyDamage, int maxX, int maxY)
    : Character(startX, startY, allyHealth, maxX, maxY), damage(allyDamage) {
}

int Ally::getDamage() const {
    return damage;
}

void Ally::move(Game &game) {
    static const std::vector<std::pair<int, int> > directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    int currentX, currentY;
    getPosition(currentX, currentY);

    for (const auto &dir: directions) {
        int newX = currentX + dir.first;
        int newY = currentY + dir.second;

        if (game.isPositionValidAndFree(newX, newY)) {
            setPosition(newX, newY);
            break;
        }
    }
}
