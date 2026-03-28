#ifndef PLAYER_H
#define PLAYER_H

#include "spell.h"
#include "player_hand.h"
#include "game_field.h"
#include "book_of_spells.h"
#include <string>
#include <vector>
#include <memory>

class Enemy;

class Player {
public:
    Player(HandSpell& hand);
    
    void move(int x, int y);
    void takeDamage(int damage);
    void switchSpell(const std::string& spellName);
    bool isAlive() const;
    void attackPosition(int targetX, int targetY, std::vector<Enemy>& enemies);
    void collectSpell(GameField& field, BookOfSpells& book, HandSpell& hand);
    void castSpell(int targetX, int targetY, std::vector<Enemy>& enemies);
    
    int getHealth() const;
    int getDamage() const;
    int getX() const;
    int getY() const;
    std::string getCurrentSpell() const;
    HandSpell& getHandSpell();

private:
    int health;
    int x, y;
    HandSpell& handSpell;
    std::unique_ptr<Spell> currentSpell;
};

#endif