#include "player.h"
#include "gamefield.h"
#include "enemy.h"
#include <iostream>
#include "spellcard.h"  
Player::Player(int handSize) : health(100), maxHealth(100), 
    combat(25, 15), economy(100, 100), spellHand(handSize) {
     
}
int Player::getHealth() const { return health; }
bool Player::isAlive() const { return health > 0; }
void Player::takeDamage(int dmg) { health -= dmg; if (health < 0) health = 0; }

CombatSystem& Player::getCombatSystem() { return combat; }
PlayerEconomy& Player::getEconomy() { return economy; }
SpellHand& Player::getSpellHand() { return spellHand; }
SpellShop& Player::getSpellShop() { return spellShop; }

bool Player::move(GameField& field, int dx, int dy) {
    int currentX, currentY;
    if (!field.findPlayerPosition(currentX, currentY)) return false;
    
    int newX = currentX + dx;
    int newY = currentY + dy;
    
    if (field.moveObject(currentX, currentY, newX, newY)) {
        economy.addScore(10);
        economy.addMana(5);
        return true;
    }
    return false;
} 
bool Player::castSpell(GameField& field, int targetX, int targetY, Enemy& enemy) {
    auto selectedSpell = spellHand.getSelectedSpell();
    if (!selectedSpell) {
        std::cout << "Нет заклинаний!\n";
        return false;
    }
     
    SpellContext context(*this, field, enemy, targetX, targetY);
    if (selectedSpell->cast(context)) {
        updateSpellCooldowns();
        return true;
    }
    return false;
}

void Player::updateSpellCooldowns() {
    spellHand.updateCooldowns();
}

void Player::openSpellShop() {
    spellShop.displayShop();
}

bool Player::buySpellFromShop(int index) {
    if (!economy.spendScore(100)) {
        std::cout << "Недостаточно очков! Нужно 100, у вас: " << economy.getScore() << "\n";
        return false;
    }
    
    if (spellHand.isFull()) {
        std::cout << "Рука заклинаний заполнена! Максимум: " << spellHand.getMaxSize() << "\n";
        economy.addScore(100);
        return false;
    }
    
    try {
        auto spell = spellShop.buySpell(index);
        if (spellHand.addSpell(spell)) {
            std::cout << "Куплено: " << spell->getName() << "!\n";
            std::cout << "Осталось очков: " << economy.getScore() << "\n";
            return true;
        }
    } catch (const std::out_of_range& e) {
        std::cout << "Неверный выбор заклинания!\n";
        economy.addScore(100);
    }
    
    return false;
}

bool Player::buySpell(int cost) {
    if (!economy.spendScore(cost)) {
        std::cout << "Недостаточно очков для покупки заклинания! Нужно: " << cost << "\n";
        return false;
    }
    
    if (spellHand.isFull()) {
        std::cout << "Рука заклинаний заполнена!\n";
        economy.addScore(cost);
        return false;
    }
    
    std::cout << "Потрачено " << cost << " очков на новое заклинание.\n";
    return spellHand.addRandomSpell();
}

int Player::getMaxHealth() const { 
    return maxHealth; 
}

void Player::restoreHealth(int amount) {
    health += amount;
    if (health > maxHealth) {
        health = maxHealth;
    }
    if (health < 0) {
        health = 0;
    }
}


void Player::setHealth(int newHealth) {
    health = newHealth;
    if (health > maxHealth) health = maxHealth;
    if (health < 0) health = 0;
}

void Player::clearSpells() { 
    spellHand = SpellHand(spellHand.getMaxSize());  
}

void Player::copySpellsFrom(const Player& other) { 
    spellHand.copySpellsFrom(other.getSpellHand());
}

const CombatSystem& Player::getCombatSystem() const { return combat; }
const PlayerEconomy& Player::getEconomy() const { return economy; }
const SpellHand& Player::getSpellHand() const { return spellHand; }
const SpellShop& Player::getSpellShop() const { return spellShop; }


