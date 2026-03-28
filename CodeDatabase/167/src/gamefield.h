#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <vector>
#include <memory>
#include "cell.h"
#include "player.h"
#include "enemy.h"
#include "direction.h"

class GameField
{
private:
  std::vector<std::vector<Cell>> grid;
  int width;
  int height;
  Player *player;
  std::vector<std::unique_ptr<Enemy>> enemies;

  bool isValidPosition(int x, int y) const;
  bool isWithinBounds(int x, int y) const;

public:
  GameField(int fieldWidth, int fieldHeight);
  GameField(const GameField &other);
  GameField(GameField &&other) noexcept;

  GameField &operator=(const GameField &other);
  GameField &operator=(GameField &&other) noexcept;

  bool placePlayer(Player *playerPtr, int x, int y);
  bool addEnemy(std::unique_ptr<Enemy> enemy, int x, int y);
  bool placeEnemy(Enemy *enemy, int x, int y);

  bool movePlayer(Direction direction);
  void moveEnemies();

  int getWidth() const;
  int getHeight() const;
  const Player *getPlayer() const;
  const std::vector<std::unique_ptr<Enemy>> &getEnemies() const;
  CellType getCellType(int x, int y) const;
  bool isPlayerAlive() const;

  bool isCellEmpty(int x, int y) const;
  void display() const;

  Enemy *getEnemyAt(int x, int y);
  void applyAreaDamage(int centerX, int centerY, int areaSize, int damage, Player &caster);
  bool hasEnemyInRadius(int centerX, int centerY, int radius) const;
  bool hasEnemyAt(int x, int y) const;
  bool applyDirectDamage(int targetX, int targetY, int damage, Player &caster);

  void removeDeadEnemies();

private:
  void copyFrom(const GameField &other);
  void moveFrom(GameField &&other) noexcept;
  void clearField();
};

#endif