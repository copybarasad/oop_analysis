#ifndef IGAMEOBJECT_H
#define IGAMEOBJECT_H

class IGameObject {
public:
    virtual ~IGameObject() = default;
    virtual int getHealth() const = 0;
    virtual int getMaxHealth() const = 0;
    virtual void takeDamage(int amount) = 0;
    virtual int getPositionX() const = 0;
    virtual int getPositionY() const = 0;
    virtual bool isAlive() const = 0;
};

#endif // IGAMEOBJECT_H