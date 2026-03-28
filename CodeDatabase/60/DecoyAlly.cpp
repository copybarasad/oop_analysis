#include <DecoyAlly.hpp>
#include <Constants.hpp>
#include <cmath>
#include <Floor.hpp>

constexpr int DECOY_IDLE_LIFETIME = 3; 

DecoyAlly::DecoyAlly(int x, int y, int targetX, int targetY)
    : LivingEntity(x, y, Entity::Type::DECOY_ALLY, DECOY_HEALTH, 0),
      targetX(targetX), targetY(targetY), 
      lifetimeCounter(DECOY_IDLE_LIFETIME),
      reachedTarget(false), 
      sprite(DECOY_ALLY_SPRITE)
{}

void DecoyAlly::draw() {
    mvaddch(y, x, sprite);
}

int DecoyAlly::getExp() const {
    return 0; 
}

bool DecoyAlly::isPassable(const std::vector<std::vector<Ceil>>& ceils, int n_x, int n_y, const ScreenSize* screen, int height, int width) const {
    int worldY = n_y - screen->yMax / 2 + height / 2;
    int worldX = n_x - screen->xMax / 2 + width / 2;

    if (worldY < 0 || worldY >= height || worldX < 0 || worldX >= width) {
        return false;
    }

    Entity::Type targetType = ceils[worldY][worldX].getEntity()->getType();
    return targetType == Entity::Type::FLOOR || 
           targetType == Entity::Type::SPIKED_TRAP ||
           targetType == Entity::Type::PLAYER_TRAP;
}

void DecoyAlly::move(std::vector<std::vector<Ceil>>& ceils, int nextX, int nextY, const ScreenSize* screen, int height, int width) {
    int oldWorldY = y - screen->yMax / 2 + height / 2;
    int oldWorldX = x - screen->xMax / 2 + width / 2;

    if (oldWorldY >= 0 && oldWorldY < height && oldWorldX >= 0 && oldWorldX < width) {
        ceils[oldWorldY][oldWorldX].setEntity(std::make_shared<Floor>(x, y));
    }

    ceils[oldWorldY][oldWorldX].setEntity(std::make_shared<Floor>(x, y));

    x = nextX;
    y = nextY;

    int newWorldY = y - screen->yMax / 2 + height / 2;
    int newWorldX = x - screen->xMax / 2 + width / 2;

    if (newWorldY < 0 || newWorldY >= height || newWorldX < 0 || newWorldX >= width) {
        this->health = 0; 
        return; 
    }
    ceils[newWorldY][newWorldX].setEntity(shared_from_this());
}

void DecoyAlly::update(std::vector<std::vector<Ceil>>& ceils, const ScreenSize* screen, int height, int width) {

    if (!reachedTarget && x == targetX && y == targetY) {
        reachedTarget = true;
    }

    int x, y;
    x = this->x;
    y = this->y;

    if (reachedTarget) {

        if (lifetimeCounter > 0) {
            lifetimeCounter--;
        } else {

            int worldY = y - screen->yMax / 2 + height / 2;
            int worldX = x - screen->xMax / 2 + width / 2;
            ceils[worldY][worldX].setEntity(std::make_shared<Floor>(x, y));

            this->health = 0; 
        }
    } else {

        int dx = (targetX > x) ? 1 : (targetX < x ? -1 : 0);
        int dy = (targetY > y) ? 1 : (targetY < y ? -1 : 0);

        if (dx != 0 && dy != 0) {
            if (isPassable(ceils, x + dx, y + dy, screen, height, width)) {
                move(ceils, x + dx, y + dy, screen, height, width);
            } else if (isPassable(ceils, x + dx, y, screen, height, width)) {
                move(ceils, x + dx, y, screen, height, width);
            } else if (isPassable(ceils, x, y + dy, screen, height, width)) {
                move(ceils, x, y + dy, screen, height, width);
            }
        } else {
            if (dx != 0 && isPassable(ceils, x + dx, y, screen, height, width)) {
                move(ceils, x + dx, y, screen, height, width);
            } else if (dy != 0 && isPassable(ceils, x, y + dy, screen, height, width)) {
                move(ceils, x, y + dy, screen, height, width);
            }
        }
    }

    if (x == this->x && y == this->y && reachedTarget != true) {
        reachedTarget = true;
    }
}

int DecoyAlly::getTargetX() const {
    return targetX;
}

int DecoyAlly::getTargetY() const {
    return targetY;
}

int DecoyAlly::getLifeTimeCounter() const {
    return lifetimeCounter;
}

bool DecoyAlly::getReachedTarget() const {
    return reachedTarget;
}

void DecoyAlly::setState(int lifetime, bool reached) {
    this->lifetimeCounter = lifetime;
    this->reachedTarget = reached;
}