#ifndef ENTITY_H
#define ENTITY_H

class Entity {
protected:
    int x, y;
    int hp;

public:
    Entity(int x = 0, int y = 0, int hp = 0);
    virtual ~Entity() = default;
    
    int getX() const;
    int getY() const;
    void setPosition(int x, int y);
    int getHp() const { return hp; }
    void setHp(int value) { hp = value; }
    void takeDamage(int dmg) { hp = hp - dmg < 0 ? 0 : hp - dmg; }
    bool isAlive() const { return hp > 0; }
};

#endif 
