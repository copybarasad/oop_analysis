#ifndef ENEMYTOWER_H
#define ENEMYTOWER_H

class GameField;
class Player;

class EnemyTower {
private:
    GameField* field;  // Первое - указатели
    int health;
    int maxHealth;
    int x;
    int y;
    int attackRange;
    int attackDamage;
    bool canAttack;
    bool alive;

public:
    EnemyTower(GameField* gameField, int posX, int posY, int towerHealth = 100, int damage = 8, int range = 4);
    
    // Getters
    int getHealth() const;
    int getMaxHealth() const;
    int getX() const;
    int getY() const;
    int getAttackRange() const;
    int getAttackDamage() const;
    bool canAttackThisTurn() const;
    bool isAlive() const;
    
    // Actions
    void takeDamage(int damageAmount);
    void attackPlayer(Player& player);
    void resetAttack();
    void update(Player& player);
};

#endif