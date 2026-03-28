#ifndef ENEMY_H
#define ENEMY_H

#include "Position.h"

class Enemy {
    private:
        int health;
        int maxHealth;
        int damage;
        Position position;

        static const int DEFAULT_HEALTH = 30;
        static const int DEFAULT_DAMAGE = 10;

    public:
        Enemy(const Position& startPosition, int enemyHealth = DEFAULT_HEALTH, int enemyDamage = DEFAULT_DAMAGE);

        // === AI Movment ===
        bool moveTowards(const Position& targetPosition);

        // === Fight System ===
        int getDamage() const;
        void takeDamage(int damage);

        // === Getters (info) ===
        Position getPosition() const;
        int getHealth() const;
        int getMaxHealth() const;
        bool isAlive() const;

        // === Setters ===
        void setPosition(const Position& newPosition);
        
        // === Setters for Save/Load ===
        void setHealth(int h);
        void setMaxHealth(int h);
        void setDamage(int d);

        void loadState(const nlohmann::json& j);
};

#endif