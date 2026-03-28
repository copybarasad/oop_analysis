#ifndef POSITIONABLE_HPP
#define POSITIONABLE_HPP

struct Position {
    int x;
    int y;
};

class Positionable {
  public:
    Position getPosition();
    void setPosition(const Position pos);

  protected:
    Position m_Pos;
};

#endif /* POSITIONABLE_HPP */
