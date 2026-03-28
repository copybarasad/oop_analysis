#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "spellhand.h"
#include "spellshop.h"
#include "combatsystem.h"
#include "playereconomy.h"

class GameField;
class Enemy;

class Player {
private:
    int health;
    int maxHealth;
    CombatSystem combat;
    PlayerEconomy economy;
    SpellHand spellHand;
    SpellShop spellShop;

public:
    Player(int handSize = 3);
    
    int getHealth() const;
    int getMaxHealth() const;
    bool isAlive() const;
    void takeDamage(int damage);
    void restoreHealth(int amount);
    
    void setHealth(int newHealth);
    void clearSpells();
    void copySpellsFrom(const Player& other);
    
    CombatSystem& getCombatSystem();
    const CombatSystem& getCombatSystem() const;
    PlayerEconomy& getEconomy();
    const PlayerEconomy& getEconomy() const;
    SpellHand& getSpellHand();
    const SpellHand& getSpellHand() const;
    SpellShop& getSpellShop();
    const SpellShop& getSpellShop() const;
    
    // УБИРАЕМ ЭТОТ МЕТОД:
    // void displayStatus(const GameField& field) const;
    
    bool move(GameField& field, int dx, int dy);
    bool castSpell(GameField& field, int targetX, int targetY, Enemy& enemy);
    void updateSpellCooldowns();
    void openSpellShop();
    bool buySpellFromShop(int index);
    bool buySpell(int cost = 100);
};

#endif