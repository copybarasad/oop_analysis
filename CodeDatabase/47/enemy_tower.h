#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

class Game;

class EnemyTower {
private:
    static const int TOWER_HEALTH = 8;
    static const int ATTACK_COOLDOWN = 3;
    static const int ATTACK_RANGE = 4;
    static const int SPELL_DAMAGE = 1;

    int positionX;
    int positionY;
    int health;
    int currentCooldown;

public:
    EnemyTower(int startX, int startY);
    
    int getPositionX() const;
    int getPositionY() const;
    int getHealth() const;
    int getCurrentCooldown() const;
    int getAttackRange() const;

    void takeDamage(int damageAmount);
    void update();
    bool canAttack() const;
    void performAttack(Game& game);
    void resetCooldown();
    bool isAlive() const;
    void setPosition(int x, int y);
    void setHealth(int h);
    void setCooldown(int c);
    bool isPlayerInRange(int playerX, int playerY) const;
};

#endif