#ifndef ENTITY_H
#define ENTITY_H

#include <string>

class GameField;

enum AttackMode { MELEE, RANGED };
enum EntityGroup { EVIL, HERO };

class Entity{
private:
    int maxHp;
    int hp;
    int posX;
    int posY;
    std::string name;
    char symbol;
    EntityGroup group;

protected:
    int dmg;
    int xp;

public:
    static GameField* currentField;
    bool alive;

public:
    Entity(int hp, int posX = 0, int posY = 0, std::string name = "Entity", char symbol = '0', EntityGroup group = EVIL, int dmg = 0, int xp = 0);
    
    void move(int dx, int dy, GameField &field);
    void attack(Entity* entity);

    int getPosX () const;
    int getPosY () const;
    char getSym () const;
    int getHp () const;
    void setMaxHp(int hp) { maxHp = hp; }
    int getMaxHp() const;
    void increaseMaxHp(int amount);
    int getXp() const;
    EntityGroup getGroup() const;

    void setPos(int x, int y);
    void setHp(int hp);
    void setXp(int xp);
    void setAlive(bool alive);

    bool isAlive() const;

    void takeDamage(int dmg);

    const std::string &getName() const;
    void setEntityOnField(GameField & field);
    };

#endif