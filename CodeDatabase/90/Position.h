#ifndef RPG_POSITION_H_
#define RPG_POSITION_H_

class Position {
private:
  int x;
  int y;

public:
  Position();
  Position(int x, int y);

  int getX() const;
  int getY() const;
  void set(int newX, int newY);
  void set(const Position &other);
  void moveUp();
  void moveDown();
  void moveLeft();
  void moveRight();

  int distanceTo(const Position &other) const;
  bool operator==(const Position &other) const;
  bool operator!=(const Position &other) const;
};

#endif // RPG_POSITION_H_
