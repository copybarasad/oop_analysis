#ifndef CHARACTER_H
#define CHARACTER_H

#include <utility>
class Character{
public:
    Character(int hp, int damage, std::pair<int, int> pos);
    virtual ~Character();

    std::pair<int, int> position() const;
    void setPosition(std::pair<int, int> p);

    int hp() const;
    int damage() const;
    bool isAlive();
    void takeDamage(int amount);
    void heal(int amount);

    void setHp(int hp);

protected:
    void moveBy(std::pair<int, int> del);
    std::pair<int, int> pos_;
    int hp_;
    int damage_;
};


#endif