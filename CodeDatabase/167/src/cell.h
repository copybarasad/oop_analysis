#ifndef CELL_H
#define CELL_H

class Player;
class Enemy;

enum class CellType
{
  EMPTY,
  PLAYER,
  ENEMY,
  WALL
};

class Cell
{
private:
  CellType type;
  Player *playerRef;
  Enemy *enemyRef;

public:
  Cell();

  CellType getType() const;
  bool isEmpty() const;
  bool isOccupied() const;

  void setPlayer(Player *player);
  void setEnemy(Enemy *enemy);
  void clear();

  bool hasPlayer() const;
  bool hasEnemy() const;
};

#endif