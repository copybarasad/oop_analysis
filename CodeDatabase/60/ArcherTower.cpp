#include "ArcherTower.hpp"
#include "Constants.hpp"
#include <cmath>

ArcherTower::ArcherTower(int x, int y) 
    : LivingEntity(x, y, Entity::Type::ARCHER_TOWER, TOWER_HEALTH, 50),
      currentState(State::IDLE),
      attackCooldown(TOWER_COOLDOWN),
      currentCooldown(0),
      attackRadius(TOWER_ATTACK_RADIUS),
      sprite(ARCHER_TOWER_SPRITE)
{}

void ArcherTower::draw() {
    mvaddch(y, x, sprite);
}

int ArcherTower::getExp() const {
    return exp;
}

bool ArcherTower::isPlayerInRange(int playerX, int playerY) const {
    return std::abs(playerX - this->x) <= attackRadius && std::abs(playerY - this->y) <= attackRadius;
}

void ArcherTower::update(std::shared_ptr<Player> player, const std::vector<std::vector<Ceil>>& ceils, const ScreenSize* screen, int height, int width) {
    if (!player) {
        return;
    }

    bool playerIsInRange = isPlayerInRange(player->getX(), player->getY());

    if (playerIsInRange) {
        player->setTargeted(true);

    }

    if (currentCooldown > 0) {
        currentCooldown--;
        if (currentCooldown == 0 && currentState == State::COOLDOWN) {
            currentState = State::IDLE;
        }
    }

    switch (currentState) {
        case State::IDLE:
            if (playerIsInRange && currentCooldown == 0) {
                currentState = State::AIMING;
            }
            break;

        case State::AIMING:
            if (playerIsInRange) {
                attackSpell.apply(player);
                currentState = State::COOLDOWN;
                currentCooldown = attackCooldown;
            } else {
                currentState = State::IDLE;
            }
            break;

        case State::COOLDOWN:
            break;
    }
}

std::string ArcherTower::getStateAsString() const {
    switch (currentState) {
        case State::IDLE:    return "IDLE";
        case State::AIMING:  return "AIMING";
        case State::COOLDOWN: return "COOLDOWN";
        default:           return "UNKNOWN";
    }
}

int ArcherTower::getCurrentCooldown() const {
    return currentCooldown;
}

void ArcherTower::setState(const std::string& stateStr, int cooldown) {
    if (stateStr == "IDLE") this->currentState = State::IDLE;
    else if (stateStr == "AIMING") this->currentState = State::AIMING;
    else if (stateStr == "COOLDOWN") this->currentState = State::COOLDOWN;

    this->currentCooldown = cooldown;
}