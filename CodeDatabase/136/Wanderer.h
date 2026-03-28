#ifndef WANDERER_H
#define WANDERER_H
 
#include "Hand.h"

enum class CombatMode {
    MELEE,
    RANGED
};

class Wanderer {
    private:
        int hp;
        int maxHp;
        int meleeDamage;
        int rangeDamage;
        int shield;
        int maxShield;
        int shieldRegen;
        int points;
        int lvl;
        int exp;
        int expToNextLvl;
        int mana;
        int maxMana;
        int manaRegen;
        Hand hand;
        CombatMode combatMode;
        bool isAlive;
        
        void lvlUp();
        void die();

    public:
        Wanderer();
        
        void takeDamage(int damageAmount);
        void heal(int healAmount);
        void addPoints(int pointsToAdd);
        void addExp(int expToAdd);
        void switchCombatMode();
        void regenerateShield();
        bool spendMana(int amount);
        void regenerateMana();
        void addMana(int amount);
        
        void setMaxHealthDirectly(int maxHealth) { this->maxHp = maxHealth; }
        void setMaxShieldDirectly(int maxShield) { this->maxShield = maxShield; }
        void setMaxManaDirectly(int maxMana) { this->maxMana = maxMana; }
        void setCombatMode(CombatMode mode) { this->combatMode = mode; }
        void setHealthDirectly(int health);
        void setManaDirectly(int mana);
        void setShieldDirectly(int shield);
        void setPointsDirectly(int points);
        void setLevelDirectly(int level, int exp, int expToNext);
        
        int getShield() const;
        int getMaxShield() const;
        int getHealth() const;
        int getMaxHealth() const;
        int getDamage() const;
        int getPoints() const;
        int getLvl() const;
        int getExp() const;
        int getExpToNextLvl() const;
        int getMana() const;
        int getMaxMana() const;
        Hand& getHand();
        const Hand& getHand() const;
        bool learnSpell(std::unique_ptr<SpellCard> spell);
        CombatMode getCombatMode() const;
        bool getIsAlive() const;
        
        void displayStatus() const;
        int getCurrentDamage() const;
};

#endif