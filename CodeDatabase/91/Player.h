#ifndef PLAYER_H
#define PLAYER_H

#include "Position.h"
#include "CombatType.h"
#include "Enemy.h"
#include "Spells/SpellHand.h"
#include <string>
#include <vector>

class Enemy;

class Player {
    private:
        std::string name;
        int health;
        int maxHealth;
        int baseDamage;
        Position position;
        CombatType combatType;
        int score;

        SpellHand spellHand;
        int mana;
        int maxMana;
        int enemiesDefeated;
        int spellPurchaseCost;

        static const int DEFAULT_HEALTH = 100;
        static const int DEFAULT_DAMAGE = 15;
        static const int MELEE_DAMAGE_MULTIPLIER = 2;
        static const int RANGED_DAMAGE_MULTIPLIER = 1;
        static const int DEFAULT_MANA = 50;
        static const int DEFAULT_MAX_MANA = 100;
        static const int MANA_PER_KILL = 10;
        static const int DEFAULT_SPELL_COST = 50;

    public:
        Player(const std::string& playerName, const Position& startPosition);

        // === Movement ===
        bool move(int deltaX, int deltaY);

        // === Fight System ===
        void switchCombatType();
        int getDamage() const;
        void takeDamage(int damage);

        // === Spell System ===
        bool castSpell(int spellIndex, const Position& target);
        void addRandomSpell();
        bool canBuySpell() const;
        bool buySpell();
        void restoreMana(int amount);
        void spendMana(int amount);
        
        // === Counters ===
        void incrementEnemiesDefeated();
        void addScore(int points);

        // === Find Enemies ===
        std::vector<Position> findEnemiesInRange(const std::vector<Enemy>& enemies, int range) const;
        Position findClosestEnemyInRange(const std::vector<Enemy>& enemies, int range) const;
        int distanceTo(const Position& pos) const;

        // === Getters ===
        Position getPosition() const;
        int getHealth() const;
        int getMaxHealth() const;
        CombatType getCombatType() const;
        std::string getCombatTypeString() const;
        int getScore() const;
        bool isAlive() const;
        std::string getName() const;
        
        SpellHand& getSpellHand();
        const SpellHand& getSpellHand() const;
        int getMana() const;
        int getMaxMana() const;
        int getEnemiesDefeated() const;
        int getSpellPurchaseCost() const;

        // === Setters ===
        void setPosition(const Position& newPosition);
        void heal(int amount);

        // === Setters for Save/Load ===
        void setHealth(int h);
        void setMaxHealth(int h);
        void setBaseDamage(int d);
        void setMana(int m);
        void setMaxMana(int m);
        void setEnemiesDefeated(int e);
        void setSpellPurchaseCost(int c);
        void setScore(int s);
        void setCombatType(CombatType ct);

        void upgradeMaxHealth(int amount);
        void upgradeBaseDamage(int amount);
        void upgradeMaxMana(int amount);

        void loadState(const nlohmann::json& j);
        int getBaseDamage() const;
};

#endif