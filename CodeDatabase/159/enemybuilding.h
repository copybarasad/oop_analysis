#ifndef ENEMYBUILDING_H
#define ENEMYBUILDING_H

class EnemyBuilding {
public:
  EnemyBuilding(int startX, int startY, int timer, int initialHealth);

  int getX() const;
  int getY() const;
  int getHealth() const;
  int getCurrentTimer() const;
  bool update();
  void takeDamage(int damage);
  bool isAlive() const;

private:
  int x, y;
  int timer;
  int currentTimer;
  int health;
};

#endif