#ifndef ENEMYTOWER_H
#define ENEMYTOWER_H

class EnemyTower {
public:
  EnemyTower(int startX, int startY, int initialHealth, int cooldownTurns);

  int getX() const;
  int getY() const;
  int getHealth() const;
  int getCurrentCooldown() const;
  bool isAlive() const;

  void takeDamage(int damage);
  bool update(int playerX, int playerY);

private:
  int x, y;
  int health;
  int cooldown;
  int currentCooldown;
};

#endif