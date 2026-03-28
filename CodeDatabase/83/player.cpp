#include "player.h"
#include "enemy.h"
#include "book_of_spells.h"
#include "game_defines.h"
#include <iostream>

Player::Player(HandSpell& hand) : health(START_HEALTH), x(START_X), y(START_Y), handSpell(hand) {
    if (hand.getCurrentSize() > 0) {
        currentSpell = hand.getSpell(0)->clone();
    }
}

void Player::move(int x, int y) {
    this->x = x;
    this->y = y;
}

void Player::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

void Player::switchSpell(const std::string& spellName) {
    if (spellName.empty()) {
        currentSpell.reset();
        std::cout << "No spell selected.\n";
        return;
    }
    
    Spell* spell = handSpell.getSpellForName(spellName);
    if (spell) {
        currentSpell = spell->clone();
        std::cout << "Switched to: " << spellName << std::endl;
        std::cout << "Spell info: ";
        currentSpell->display();
        std::cout << std::endl;
    } else {
        std::cout << "Spell '" << spellName << "' not found in hand!" << std::endl;
        std::cout << "Your available spells: ";
        handSpell.displayAllName();
        std::cout << std::endl;
        
        if (handSpell.getCurrentSize() > 0) {
            currentSpell = handSpell.getSpell(0)->clone();
            std::cout << "Auto-switched to: " << currentSpell->getName() << std::endl;
        } else {
            currentSpell.reset();
            std::cout << "No spells available in hand.\n";
        }
    }
}

bool Player::isAlive() const {
    return health > 0;
}

void Player::attackPosition(int targetX, int targetY, std::vector<Enemy>& enemies) {
    if (!currentSpell) {
        std::cout << "No spell selected!" << std::endl;
        return;
    }
    
    currentSpell->cast(x, y, targetX, targetY, enemies, *this);
}

void Player::collectSpell(GameField& field, BookOfSpells& book, HandSpell& hand) {
    FieldSpell* fieldSpell = field.getSpellAtPosition(x, y);
    if (fieldSpell && !fieldSpell->isCollected()) {
        auto spellToAdd = book.createSpellByName(fieldSpell->getSpellName());
        if (spellToAdd) {
            hand.addSpell(std::move(spellToAdd));
            fieldSpell->collect();
            std::cout << "You collected a spell: " << fieldSpell->getSpellName() << std::endl;
            
            if (!currentSpell) {
                currentSpell = book.createSpellByName(fieldSpell->getSpellName());
                std::cout << "Auto-equipped: " << fieldSpell->getSpellName() << std::endl;
            }
        }
    }
}

void Player::castSpell(int targetX, int targetY, std::vector<Enemy>& enemies) {
    if (!currentSpell) {
        std::cout << "No spell selected!\n";
        return;
    }
    
    std::cout << "Casting " << currentSpell->getName() << " at (" 
              << targetX << ", " << targetY << ")\n";
    
    currentSpell->cast(x, y, targetX, targetY, enemies, *this);
}

int Player::getHealth() const {
    return health;
}

int Player::getDamage() const {
    return currentSpell ? currentSpell->getDamage() : 5;
}

int Player::getX() const {
    return x;
}

int Player::getY() const {
    return y;
}

std::string Player::getCurrentSpell() const {
    return currentSpell ? currentSpell->getName() : "None";
}

HandSpell& Player::getHandSpell() {
    return handSpell;
}