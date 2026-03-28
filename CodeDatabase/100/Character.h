#pragma once

class GameField;

class Character {
protected:
    int health;
    int maxHealth;
    int damage;
    int posX, posY;
    int prevX, prevY;
    char symbol;

public:
    Character(int health, int damage, int startX, int startY, char symbol);
    virtual ~Character() = default;

    // Геттеры
    int getHealth() const;
    int getMaxHealth() const;
    int getDamage() const;
    int getX() const;
    int getY() const;
    int getPrevX() const;
    int getPrevY() const;
    char getSymbol() const;

    // Сеттеры
    void setPosition(int x, int y);
    void setHealth(int newHealth);
    void setMaxHealth(int newMaxHealth);
    void setDamage(int newDamage);
    void takeDamage(int damage);
    void heal(int amount);

    // Проверки
    bool isAlive() const;

    // Передвижение
    virtual void move(int deltaX, int deltaY, const GameField& field);
    void revertToPreviousPosition();
};