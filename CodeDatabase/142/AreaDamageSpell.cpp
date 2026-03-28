#include "AreaDamageSpell.h"
#include "Player.h"
#include "Field.h"
#include "Enemy.h"
#include <iostream>
#include <cmath>

AreaDamageSpell::AreaDamageSpell(int radius, int damage) 
    : radius(radius), damage(damage) {}

bool AreaDamageSpell::cast(Player* caster, int targetX, int targetY, Field& field) {
    if (!caster) return false;
    
    if (!isInRange(caster, targetX, targetY)) {
        std::cout << "Цель вне радиуса действия заклинания!\n";
        return false;
    }

    if (!field.isValidPosition(targetX, targetY)) {
        std::cout << "Неверная позиция цели!\n";
        return false;
    }
    
    int enemiesHit = 0;
    
    for (int dy = 0; dy < 2; dy++) {
        for (int dx = 0; dx < 2; dx++) {
            int checkX = targetX + dx;
            int checkY = targetY + dy;
            
            if (!field.isValidPosition(checkX, checkY)) {
                continue;
            }
            

            Enemy* enemy = field.getEnemyAt(checkX, checkY);
            if (enemy) {
                enemy->takeDamage(damage);
                enemiesHit++;
                
                if (!enemy->isAlive()) {
                    std::cout << "Враг в (" << checkX << ", " << checkY << ") убит заклинанием! +10 очков\n";
                    caster->addScore(10);
                    field.removeEnemy(enemy);
                } else {
                    std::cout << "Враг в (" << checkX << ", " << checkY << ") получил урон. Осталось HP: " << enemy->getHP() << "\n";
                }
            }
        }
    }
    
    std::cout << "Заклинание урона по области нанесло " << damage 
              << " урона в области 2x2 начиная с (" << targetX << ", " << targetY << ")!\n";
    if (enemiesHit > 0) {
        std::cout << "Затронуто врагов: " << enemiesHit << "\n";
    } else {
        std::cout << "В области нет врагов, но заклинание все равно использовано.\n";
    }
    
    return true;
}

const char* AreaDamageSpell::getName() const {
    return "Урон по области";
}

const char* AreaDamageSpell::getDescription() const {
    return "Наносит урон по области 2x2 клетки";
}

std::unique_ptr<Spell> AreaDamageSpell::clone() const {
    return std::make_unique<AreaDamageSpell>(radius, damage);}

int AreaDamageSpell::getRadius() const {
    return radius;
}

int AreaDamageSpell::getDamage() const {
    return damage;
}

bool AreaDamageSpell::isInRange(Player* caster, int targetX, int targetY) const {
    if (!caster) return false;
    
    int dx = std::abs(caster->getX() - targetX);
    int dy = std::abs(caster->getY() - targetY);
    
    // Using Manhattan distance for range checking
    return (dx + dy) <= radius;
}

