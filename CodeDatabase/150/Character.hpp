#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdexcept>

class Character {
protected:
    int health_;
    int maxHealth_;
    int damage_;
    int positionX_;
    int positionY_;

    void validateHealth();

public:
    Character(int x, int y, int health, int maxHealth, int damage);
    virtual ~Character() = default;

    Character(const Character& other);
    Character& operator=(const Character& other);
    Character(Character&& other) noexcept;
    Character& operator=(Character&& other) noexcept;

    virtual int getDamage() const;
    int getHealth() const;
    int getMaxHealth() const;
    int getPositionX() const;
    int getPositionY() const;

    bool isAlive() const;
    void takeDamage(int damage);
    void setPosition(int x, int y);

    virtual void serialize(std::ostream& os) const;
    virtual void deserialize(std::istream& is);
};

#endif