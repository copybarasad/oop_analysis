#ifndef UNIT_H
#define UNIT_H

class Unit {
public:
    explicit Unit(int x = 0, int y = 0, int hp = 100, int damage = 10);
    virtual ~Unit() = default;

    int getX() const;
    int getY() const;

    void setPosition(int x, int y);

    int getHP() const;
    int getDamage() const;
    void takeDamage(int d);
    bool isAlive() const;

protected:
    int x_;
    int y_;
    int hp_;
    int damage_;
};

#endif