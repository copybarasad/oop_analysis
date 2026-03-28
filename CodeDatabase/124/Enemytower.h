#ifndef ENEMYTOWER_H
#define ENEMYTOWER_H

class Player;
class GameField;

class EnemyTower {
private:
    int health;
    int damage;
    int attackRange;
    int cooldown;
    int currentCooldown;
    int x, y;
    bool isAlive;
    void attackPlayer(Player& player);
public:
    EnemyTower(int towerHealth = 75, int towerDamage = 15, int range = 4);
    
    int getHealth() const;
    int getDamage() const; 
    int getAttackRange() const;
    bool getIsAlive() const;
    void getPosition(int& outX, int& outY) const;
    
    void place(int posX, int posY);
    void takeDamage(int damage);
    void update(GameField& field, Player& player);
    bool canAttackPlayer(const GameField& field, const Player& player) const;
    void setHealth(int newHealth) {
        health = newHealth;
        if (health <= 0) {
            health = 0;
            isAlive = false;
        }
    }

};
#endif