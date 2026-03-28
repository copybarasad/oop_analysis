#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

class GameManager;

class EnemyTower {
private:
    int posX;
    int posY;
    int attackRange;
    int cooldown;
    int currentCooldown;

public:
    EnemyTower(int x, int y, int range);
    
    void update();
    bool tryAttackPlayer(GameManager& gameManager);
    int getPosX() const;
    int getPosY() const;
    bool isOnCooldown() const;
};

#endif
