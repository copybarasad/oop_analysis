#include "DirectDamageSpell.h"
#include "Player.h"
#include "Field.h"
#include "Enemy.h"
#include "Entity.h"
#include <iostream>
#include <cmath>

DirectDamageSpell::DirectDamageSpell(int radius, int damage) 
    : radius(radius), damage(damage) {}

bool DirectDamageSpell::cast(Player* caster, int targetX, int targetY, Field& field) {
    if (!caster) return false;
    
    if (!isInRange(caster, targetX, targetY)) {
        std::cout << "Цель вне радиуса действия заклинания!\n";
        return false;
    }
    
    if (!field.isValidPosition(targetX, targetY)) {
        std::cout << "Неверная позиция цели!\n";
        return false;
    }
    
    Enemy* enemy = field.getEnemyAt(targetX, targetY);
    
    if (!enemy) {
        std::cout << "В указанной позиции нет врага или вражеского здания!\n";
        return false;
    }
    
    enemy->takeDamage(damage);
    std::cout << "Заклинание прямого урона нанесло " << damage 
              << " урона врагу в позиции (" << targetX << ", " << targetY << ")!\n";
    
    if (!enemy->isAlive()) {
        std::cout << "Враг убит заклинанием! +10 очков\n";
        caster->addScore(10);
        field.removeEnemy(enemy);
    } else {
        std::cout << "У врага осталось " << enemy->getHP() << " HP\n";
    }
    
    return true;
}

const char* DirectDamageSpell::getName() const {
    return "Прямой урон";
}

const char* DirectDamageSpell::getDescription() const {
    return "Наносит урон одному врагу в радиусе действия";
}

std::unique_ptr<Spell> DirectDamageSpell::clone() const {
    return std::make_unique<DirectDamageSpell>(radius, damage);
}

int DirectDamageSpell::getRadius() const {
    return radius;
}

int DirectDamageSpell::getDamage() const {
    return damage;
}

bool DirectDamageSpell::isInRange(Player* caster, int targetX, int targetY) const {
    if (!caster) return false;
    
    int dx = std::abs(caster->getX() - targetX);
    int dy = std::abs(caster->getY() - targetY);
    
    // Using Manhattan distance for range checking
    return (dx + dy) <= radius;
}

