#pragma once
#include "character.h"
#include "hand.h"

class ConsoleRenderer;
class Field;
class Enemy;

class Player : public Character {
public:
    enum class CombatType { NEAR, FAR };

private:
    int points;
    CombatType combatType;
    int combatPower;
    int radius;
    Hand hand;
    Spell* createRandomSpell();
public:
    Player(std::string name, Field& field, int startX, int startY);
    ~Player() = default;

    void addPoints(int val);
    void removePoints(int val);
    int getPoints() const;
    int& getPointsRef() { return points; } 
    int getPower() const;
    int getRadius() const;
    CombatType getCombatType() const;

    void turnCombat(CombatType type);
    bool attack_force(Enemy& enemy, std::string& message);
    bool moveTo(int dx, int dy, std::string& message);
    bool moveUp(std::string& message);
    bool moveDown(std::string& message);
    bool moveLeft(std::string& message);
    bool moveRight(std::string& message);

    Hand& getHand();
    const Hand& getHand() const;
    bool useSpell(int idx, int x, int y, Field& field, Enemy& enemy, string& message);

    void setPoints(int newPoints) { points = newPoints; }
    void setName(std::string newName) { name = newName; }
    void setCombatPower(int power) { combatPower = power; }
    void setRadius(int newRadius) { radius = newRadius; }
    void setCombatType(CombatType type) { 
        combatType = type; 
        if (type == CombatType::NEAR) {
            combatPower = 2; 
            radius = 1;
        } else {
            combatPower = 1; 
            radius = 3;
        }
    }
};