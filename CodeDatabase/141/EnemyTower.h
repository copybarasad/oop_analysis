#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

class Game;

class EnemyTower {
private:
    int positionX;
    int positionY;
    int attackRadius;
    int damage;
    int cooldown;
    int currentCooldown;

    void validateCooldown(int towerCooldown) const;

public:
    EnemyTower(int x, int y, int radius, int towerDamage, int towerCooldown, int maxX, int maxY);

    void getPosition(int &x, int &y) const;

    int getAttackRadius() const;

    int getDamage() const;

    bool canAttack() const;

    void update();

    void attackPlayer(Game &game);

    bool isPlayerInRange(const Game &game) const;

    int getCooldown() const;

    int getCurrentCooldown() const;
};

#endif
