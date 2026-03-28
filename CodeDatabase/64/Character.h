#ifndef CHARACTER_H
#define CHARACTER_H

class Character {
protected:
    int hitPoints;
    int attackDamage;
    int coordX;
    int coordY;
    bool aliveStatus;

public:
    Character(int health, int damage, int x, int y);
    virtual ~Character() = default;

    int getHitPoints() const;
    int getAttackDamage() const;
    int getCoordX() const;
    int getCoordY() const;
    bool isAlive() const;

    virtual void receiveDamage(int damageAmount);
    virtual void changePosition(int newX, int newY);
    bool isOnPosition(int x, int y) const;
};

#endif