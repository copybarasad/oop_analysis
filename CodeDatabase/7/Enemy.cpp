#include "Enemy.h"
#include "Player.h"
#include "Field.h"
#include "Node.h"
#include <iostream>
#include <cmath>
#include <limits>

Enemy::Enemy(std::string name, Field* field, int startX, int startY, int level)
    : Character(name, field, startX, startY, 20 + (level - 1) * 10), 
      damagePower(4 + (level - 1) * 2) {
}

Enemy::Enemy(std::istream& is, Field* newField) : Character("", newField, 0, 0, 0) {
    std::getline(is, name);
    is >> lives >> maxLives >> x >> y >> damagePower;
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    this->field = newField;
}

void Enemy::moveTowards(const Player& player) {
    int playerX = player.getX();
    int playerY = player.getY();
    int dx = playerX - x;
    int dy = playerY - y;
    if (std::abs(dx) <= 1 && std::abs(dy) <= 1) {
        return; 
    }
    int moveX = (dx > 0) ? 1 : ((dx < 0) ? -1 : 0);
    int moveY = (dy > 0) ? 1 : ((dy < 0) ? -1 : 0);
    
    auto tryMoveTo = [&](int destX, int destY) {
        if (field->areCoordinatesValid(destX, destY) && field->getNode(destX, destY).isPassable() && field->getNode(destX, destY).getType() != Type::PLAYER) {
            
            if (field->getNode(destX, destY).getType() == Type::TRAP) {
                int trapDmg = field->getNode(destX, destY).triggerTrap();
                std::cout << this->getName() << " попал в ловушку и получил " << trapDmg << " урона!" << std::endl;
                this->takeDamage(trapDmg);
            }

            field->getNode(x, y).setType(Type::EMPTY);
            setPosition(destX, destY);
            if(this->isAlive()) {
                field->getNode(x, y).setType(Type::ENEMY);
            }
            return true;
        }
        return false;
    };

    if (tryMoveTo(x + moveX, y + moveY)) {}
    else if (tryMoveTo(x + moveX, y)) {}
    else if (tryMoveTo(x, y + moveY)) {}
}

int Enemy::getDamagePower() const {
    return damagePower;
}

void Enemy::save(std::ostream& os) const {
    os << name << std::endl;
    os << lives << " " << maxLives << " " << x << " " << y << " " << damagePower << std::endl;
}