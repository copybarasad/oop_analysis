#ifndef ENEMY_H
#define ENEMY_H

class Player;
class GameField;

class Enemy {
private:
    int health;
    int maxHealth;
    int damage;
    int x;
    int y;
    bool alive;
    char symbol;
    GameField* field; // ссылка на игровое поле

public:
    Enemy(GameField* gameField, int startX = 0, int startY = 0, int enemyHealth = 50, int enemyDamage = 5);
    
    // Геттеры
    int getHealth() const;
    int getMaxHealth() const;
    int getDamage() const;
    int getX() const;
    int getY() const;
    bool isAlive() const;
    char getSymbol() const;
    
    // Сеттеры
    void setPosition(int newX, int newY);
    
    // Методы действий
    void moveTowardsPlayer(int playerX, int playerY, Player& player);
    void takeDamage(int damageAmount);
    void displayStatus() const;
    
private:
    bool isValidMove(int newX, int newY) const;
};

#endif