#include <Enemy.hpp>
#include <Floor.hpp>
#include <Constants.hpp>
#include <cmath>
#include <BattleController.hpp>
#include <PlayerTrap.hpp>
#include <DecoyAlly.hpp>
#include <LivingEntity.hpp>

Enemy::Enemy(int x, int y, int level) : LivingEntity(x, y, Entity::Type::ENEMY, EN_HEALTH + (level - 1) * 15, 100 + (level - 1) * 20), damage(EN_DAMAGE + (level - 1) * 3), sprite(ENEMY), level(level) {}

void Enemy::move(std::vector<std::vector<Ceil>>& ceils, const ScreenSize* screen, int newX, int newY, int height, int width) {

    int newWorldY = newY - screen->yMax / 2 + height / 2;
    int newWorldX = newX - screen->xMax / 2 + width / 2;

    if (newWorldY < 0 || newWorldY >= height || newWorldX < 0 || newWorldX >= width) {
        return;
    }

    auto& targetCell = ceils[newWorldY][newWorldX];
    auto targetEntity = targetCell.getEntity();

    if (targetEntity->getType() == Entity::Type::PLAYER_TRAP) {

        auto trap = std::static_pointer_cast<PlayerTrap>(targetEntity);
        this->takeDamage(trap->getDamage()); 

        targetCell.setEntity(std::make_shared<Floor>(newX, newY));
    }

    int oldWorldY = y - screen->yMax / 2 + height / 2;
    int oldWorldX = x - screen->xMax / 2 + width / 2;
    ceils[oldWorldY][oldWorldX].setEntity(std::make_shared<Floor>(x, y));

    this->x = newX;
    this->y = newY;

    if (this->getHealth() > 0) {

        ceils[newWorldY][newWorldX].setEntity(shared_from_this());
    }
}

bool Enemy::isPassable(const std::vector<std::vector<Ceil>>& ceils, const ScreenSize* screen, int nx, int ny, int height, int width) const {
    int idxY = ny - screen->yMax / 2 + height / 2;
    int idxX = nx - screen->xMax / 2 + width / 2;

    if (idxY < 0 || idxY >= static_cast<int>(ceils.size()) ||
        idxX < 0 || idxX >= static_cast<int>(ceils[0].size())) {
        return false;
    }

    auto entity = ceils[idxY][idxX].getEntity();
    return entity && (
        entity->getType() == Entity::Type::FLOOR ||
        entity->getType() == Entity::Type::PLAYER ||
        entity->getType() == Entity::Type::PLAYER_TRAP 
    );
}

void Enemy::update(std::vector<std::vector<Ceil>>& ceils, const ScreenSize* screen, const std::vector<std::shared_ptr<LivingEntity>>& allEntities, int height, int width) {

    std::shared_ptr<LivingEntity> target = nullptr;
    int minDistance = 9999;
    int highestPriority = 0; 

    for (const auto& entity : allEntities) {
        int currentPriority = 0;

        if (entity->getType() == Entity::Type::DECOY_ALLY) {
            currentPriority = 2;
        } else if (entity->getType() == Entity::Type::PLAYER) {
            currentPriority = 1;
        } else {
            continue;
        }

        int distance = std::abs(entity->getX() - this->x) + std::abs(entity->getY() - this->y);

        if (currentPriority > highestPriority) {
            highestPriority = currentPriority;
            minDistance = distance;
            target = entity;
        } else if (currentPriority == highestPriority && distance < minDistance) {
            minDistance = distance;
            target = entity;
        }
    }

    if (!target) return; 

    int targetX = target->getX();
    int targetY = target->getY();

    if (std::abs(this->x - targetX) <= 1 && std::abs(this->y - targetY) <= 1) {

        if (target->getType() == Entity::Type::PLAYER) {
            auto player_target = std::static_pointer_cast<Player>(target);
            if (!player_target->getWasAttacked()) {
                player_target->setWasAttacked(true);
                BattleController battle(screen, ceils, player_target, std::static_pointer_cast<Enemy>(shared_from_this()), BattleController::BattleTurn::ENEMY, height, width);
                battle.start();
            }
        } else if (target->getType() == Entity::Type::DECOY_ALLY) {
            target->takeDamage(this->attack()); 
        }
        return;
    }

    int dx = (targetX > x) ? 1 : (targetX < x ? -1 : 0);
    int dy = (targetY > y) ? 1 : (targetY < y ? -1 : 0);

    if (dx != 0 && dy != 0) {
        if (isPassable(ceils, screen, x + dx, y + dy, height, width)) {
            move(ceils, screen, x + dx, y + dy, height, width);
        } else if (isPassable(ceils, screen, x + dx, y, height, width)) {
            move(ceils, screen, x + dx, y, height, width);
        } else if (isPassable(ceils, screen, x, y + dy, height, width)) {
            move(ceils, screen, x, y + dy, height, width);
        }
    } else {
        if (dx != 0 && isPassable(ceils, screen, x + dx, y, height, width)) {
            move(ceils, screen, x + dx, y, height, width);
        } else if (dy != 0 && isPassable(ceils, screen, x, y + dy, height, width)) {
            move(ceils, screen, x, y + dy, height, width);
        }
    }
}

void Enemy::draw() {
    mvaddch(y, x, sprite);
}

void Enemy::takeDamage(int amount) {
    health-=amount;
}

int Enemy::attack() {return damage;}

int Enemy::getExp() const {return exp;}

int Enemy::getLvl() const {
    return level;
}