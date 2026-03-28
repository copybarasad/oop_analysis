#ifndef ENEMY_H
#define ENEMY_H

#include <string>

class Enemy {
private:
    int health;
    int maxHealth;
    int damage;
    int positionX;
    int positionY;
    int id;
    
    void validateInvariant() const;

public:
    Enemy(int startX, int startY, int initialHealth = 50, 
          int enemyDamage = 10, int enemyId = 0);
    
    // Getters
    int getHealth() const;
    int getMaxHealth() const;
    int getDamage() const;
    int getPositionX() const;
    int getPositionY() const;
    int getId() const;
    bool isAlive() const;
    
    // Movement and combat
    bool moveTo(int newX, int newY);
    void takeDamage(int damage);
    void attackPlayer(class Player& player);
    
    // AI behavior
    void makeMove(class GameField& field, const Player& player);
    
    // Utility methods
    void heal(int amount);
    int calculateDistanceToPlayer(const Player& player) const;
    
    // Serialization
    std::string serialize() const;
    bool deserialize(const std::string& data);
};

#endif // ENEMY_H

