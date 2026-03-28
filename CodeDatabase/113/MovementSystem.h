#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

class GameField;
class Player;

class MovementSystem {
private:
    int m_positionX;
    int m_positionY;

public:
    MovementSystem(int startX, int startY);

    bool move(int deltaX, int deltaY, GameField& field, Player& player);
    bool tryMoveTo(int newX, int newY, GameField& field, Player& player);

    int getPositionX() const;
    int getPositionY() const;
    void setPosition(int x, int y);

    bool isValidMove(int x, int y, const GameField& field) const;

private:
    void handleTrapTrigger(int x, int y, GameField& field, Player& player);
};

#endif // MOVEMENTSYSTEM_H