#pragma once
#include <string>

class Field;

class Character {
protected:
    std::string name;
    int lives;
    int maxLives;
    int damage;
    int x, y;
    Field* field;

public:
    Character(std::string name, Field* field, int startX, int startY, int initialLives);
    virtual ~Character() = default;
    void takeDamage(int amount);
    bool isAlive() const;
    int getX() const;
    int getY() const;
    int getLives() const;
    int getMaxLives() const; // максимальное здоровье
    std::string getName() const;
    void restoreHealth(); // восстановление здоровья

    void setField(Field* newField);

protected:
    void setPosition(int newX, int newY);
};