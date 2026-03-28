#ifndef ALLY_H
#define ALLY_H

#include <vector>
#include <string>

class Ally {
private:
    int health;
    int maxHealth;
    int damage;
    int positionX;
    int positionY;
    int id;
    bool alive;

public:
    Ally(int startX, int startY, int initialHealth = 30, int allyDamage = 5, int allyId = 0);
    
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
    void attackEnemy(class Enemy& enemy);
    
    // AI behavior
    void makeMove(class GameField& field, const class Player& player, std::vector<class Enemy>& enemies);
    
    // Utility methods
    void heal(int amount);
    int calculateDistanceToPlayer(const class Player& player) const;
    
    // Serialization
    std::string serialize() const;
    bool deserialize(const std::string& data);
};

#endif // ALLY_H

