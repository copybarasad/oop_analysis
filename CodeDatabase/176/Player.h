#ifndef PLAYER_H
#define PLAYER_H

#include "GameEnums.h"
#include "SpellEnhancementManager.h" 
#include "SpellHand.h"
#include "SpellEnhancementManager.h"
#include <memory>

class Player {
private:
    int maxHealth;
    int currentHealth;
    int meleeDamage;
    int rangedDamage;
    int score;
    int enemiesKilled;
    int mana;
    int maxMana;
    CombatMode currentCombatMode;
    bool isSlowed;
    std::unique_ptr<SpellHand> spellHand;
    SpellEnhancementManager enhancementManager;
    int enemiesForNextSpell;
    

public:
    Player(int health, int meleeDmg, int rangedDmg, int playerMana, size_t spellHandSize);
    
    int getHealth() const;
    int getDamage() const;
    int getScore() const;
    int getEnemiesKilled() const;
    int getMana() const;
    int getMaxMana() const;
    CombatMode getCombatMode() const;
    bool getIsSlowed() const;
    SpellHand* getSpellHand() const;
    SpellEnhancementManager& getEnhancementManager();
    const SpellEnhancementManager& getEnhancementManager() const;
    int getEnemiesForNextSpell() const;
    void restoreHealth() {
            currentHealth = maxHealth;
    }
    void takeDamage(int damage);
    void addScore(int points);
    void addEnemyKilled();
    void switchCombatMode();
    void setSlowed(bool slowed);
    void addMana(int amount);
    bool useMana(int amount);
    void regenerateMana();
    bool isAlive() const;
};

#endif
