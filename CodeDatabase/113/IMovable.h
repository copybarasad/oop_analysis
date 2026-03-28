#ifndef IMOVABLE_H
#define IMOVABLE_H

class GameField;
class Player;

class IMovable {
public:
    virtual ~IMovable() = default;
    virtual void makeMove(GameField& field, Player& player) = 0;
};

#endif // IMOVABLE_H