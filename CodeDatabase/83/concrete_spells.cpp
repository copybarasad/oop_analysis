#include "concrete_spells.h"
#include "enemy.h"
#include "player.h"
#include "player_hand.h"
#include <iostream>
#include <cmath>
#include <cstdlib>

BaseSpell::BaseSpell(const std::string& name, int damage, int range, int manaCost)
    : name(name), damage(damage), range(range), manaCost(manaCost), level(1) {}

void BaseSpell::display() const {
    std::cout << name << " [Damage: " << damage << ", Range: " << range 
              << ", Mana: " << manaCost << ", Level: " << level << "]";
}

bool BaseSpell::isInRange(int casterX, int casterY, int targetX, int targetY) const {
    int distance = std::max(std::abs(casterX - targetX), std::abs(casterY - targetY));
    return distance <= range;
}

void BaseSpell::applyDamageToEnemyAt(int x, int y, std::vector<Enemy>& enemies) {
    for (auto& enemy : enemies) {
        if (enemy.isAlive() && enemy.getX() == x && enemy.getY() == y) {
            enemy.takeDamage(damage);
            std::cout << name << " hit enemy at (" << x << ", " << y 
                      << ")! Damage: " << damage 
                      << ", Enemy HP: " << enemy.getHealth() << "\n";
            return;
        }
    }
}

void BaseSpell::cast(int casterX, int casterY, int targetX, int targetY,
                    std::vector<Enemy>& enemies, Player& player) {
    if (!isInRange(casterX, casterY, targetX, targetY)) {
        std::cout << "Target too far for " << name << "! Range: " << range 
                  << ", Distance: " << std::max(std::abs(casterX - targetX), std::abs(casterY - targetY)) << "\n";
        return;
    }
    
    std::cout << "Casting " << name << " at position (" << targetX << ", " << targetY << ")\n";
    applyDamageToEnemyAt(targetX, targetY, enemies);
}

void BaseSpell::upgrade() {
    level++;
    damage += 5;
    std::cout << name << " upgraded to level " << level << "! Damage: " << damage << "\n";
}

FireballSpell::FireballSpell() 
    : BaseSpell("Fireball", 20, 5, 20) {}

std::unique_ptr<Spell> FireballSpell::clone() const {
    return std::make_unique<FireballSpell>();
}

void FireballSpell::cast(int casterX, int casterY, int targetX, int targetY,
                        std::vector<Enemy>& enemies, Player& player) {
    if (!isInRange(casterX, casterY, targetX, targetY)) {
        std::cout << "Target too far for Fireball! Range: " << range 
                  << ", Distance: " << std::max(std::abs(casterX - targetX), std::abs(casterY - targetY)) << "\n";
        return;
    }
    
    std::cout << "Casting Fireball at position (" << targetX << ", " << targetY << ")\n";
    std::cout << "Fireball explodes, damaging enemies in 3x3 area!\n";
    
    int hits = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int checkX = targetX + dx;
            int checkY = targetY + dy;
            
            for (auto& enemy : enemies) {
                if (enemy.isAlive() && enemy.getX() == checkX && enemy.getY() == checkY) {
                    enemy.takeDamage(damage);
                    std::cout << "   Fireball hit enemy at (" << checkX << ", " << checkY 
                              << ")! Damage: " << damage 
                              << ", Enemy HP: " << enemy.getHealth() << "\n";
                    hits++;
                }
            }
        }
    }
    
    if (hits == 0) {
        std::cout << "Fireball exploded but hit no enemies!\n";
    } else {
        std::cout << "Total enemies hit: " << hits << "\n";
    }
}

IceBladeSpell::IceBladeSpell() 
    : BaseSpell("Ice Blade", 15, 1, 10) {}

std::unique_ptr<Spell> IceBladeSpell::clone() const {
    return std::make_unique<IceBladeSpell>();
}

EnlighteningSpell::EnlighteningSpell() 
    : BaseSpell("Enlightening", 0, 0, 5) {}

std::unique_ptr<Spell> EnlighteningSpell::clone() const {
    return std::make_unique<EnlighteningSpell>();
}

void EnlighteningSpell::cast(int casterX, int casterY, int targetX, int targetY,
                            std::vector<Enemy>& enemies, Player& player) {
    std::cout << "Enlightening spell - choose a spell to upgrade:\n";
    
    HandSpell& hand = player.getHandSpell();
    hand.displayAllName();
    
    std::string spellName;
    std::cout << "Enter spell name to upgrade: ";
    std::cin >> spellName;
    
    Spell* spellToUpgrade = hand.getSpellForName(spellName);
    if (spellToUpgrade) {
        spellToUpgrade->upgrade();
    } else {
        std::cout << "Cannot upgrade " << spellName << "!\n";
    }
}

void EnlighteningSpell::upgrade() {
    level++;
    std::cout << name << " upgraded to level " << level << "! (Improves upgrade efficiency)\n";
}

IllusionSpell::IllusionSpell() 
    : BaseSpell("Illusion", 20, 3, 20) {}

std::unique_ptr<Spell> IllusionSpell::clone() const {
    return std::make_unique<IllusionSpell>();
}

TowerBoltSpell::TowerBoltSpell() 
    : BaseSpell("Tower Bolt", 12, 4, 0) {}

std::unique_ptr<Spell> TowerBoltSpell::clone() const {
    return std::make_unique<TowerBoltSpell>();
}

void TowerBoltSpell::cast(int casterX, int casterY, int targetX, int targetY,
                         std::vector<Enemy>& enemies, Player& player) {
    int distance = std::max(std::abs(casterX - targetX), std::abs(casterY - targetY));
    
    if (distance <= getRange()) {
        std::cout << "Tower casts " << getName() << " at player!\n";
        player.takeDamage(getDamage());
        std::cout << "Tower Bolt hit player! Damage: " << getDamage() 
                  << ", Player HP: " << player.getHealth() << "\n";
    }
}