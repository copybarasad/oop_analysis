#include "Tower.h"
#include "Player.h"
#include "Spell.h"
#include "Field.h"
#include <iostream>
#include <cmath>

class WeakDirectSpell : public Spell {
private:
    int damage;

public:
    WeakDirectSpell(int dmg = 12) : damage(dmg) {}
    
    void cast(Player& player, std::vector<Enemy>& enemies, Field& field) override {
        std::cout << "Башня применяет ослабленное заклинание прямого урона!\n";
        std::cout << "Игрок получает " << damage << " урона!\n";
        player.takeDamage(damage);
    }
    
    std::string getName() const override {
        return "Ослабленный магический снаряд";
    }
};

Tower::Tower(int X, int Y, int rg, int hp, int cd)
    : x(X), y(Y), range(rg), health(hp), cooldown(cd), 
      currentCooldown(0), alive(true) {

    weakSpell = std::make_unique<WeakDirectSpell>(12);
    
    std::cout << "   Вражеская башня построена на (" << x << ", " << y << ")\n";
    std::cout << "   Заклинание: " << weakSpell->getName() << "\n";
    std::cout << "   Радиус: " << range << ", HP: " << health << "\n";
}

void Tower::update(Player& player, std::vector<Enemy>& enemies, Field& field) {
    if (!alive) return;
    
    if (currentCooldown > 0) {
        currentCooldown--;
        std::cout << "Башня перезаряжается... " << currentCooldown << " ходов до готовности!\n";      
    }

    if (canAttack() && playerInRange(player)) {
        std::cout << " Башня заметила игрока в радиусе " << range << " клеток!\n";
        
        if (weakSpell) {
            weakSpell->cast(player, enemies, field);
        }
        
        currentCooldown = cooldown;
        std::cout << "Башня уходит на перезарядку на " << cooldown << " ходов\n";
    }
}

void Tower::takeDamage(int dmg) {
    if (!alive) return;

    health -= dmg;
    std::cout << "Башня получает " << dmg << " урона!\n";
    std::cout << "HP башни: " << health << "\n";

    if (health <= 0) {
        health = 0;
        alive = false;
        std::cout << "Башня разрушена!\n";
    }
}

bool Tower::canAttack() const {
    return alive && currentCooldown == 0;
}

bool Tower::playerInRange(const Player& player) const {
    if (!alive) return false;

    int plX = player.getX();
    int plY = player.getY();

    int dist = std::abs(plX - x) + std::abs(plY - y);
    return dist <= range;
}

int Tower::getX() const { return x; }
int Tower::getY() const { return y; }
int Tower::getHealth() const { return health; }
int Tower::getRange() const { return range; }
int Tower::getCooldown() const {return cooldown; }
int Tower::getCurrentCooldown() const {return currentCooldown; }
bool Tower::isAlive() const { return alive; }