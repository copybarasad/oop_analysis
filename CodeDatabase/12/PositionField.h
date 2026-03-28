#ifndef POSITIONFIELD_H
#define POSITIONFIELD_H

class Position_field {
private:
    bool HasPlayer;
    bool HasEnemy;
    bool Blockedpos;
    bool HasTrap;
    bool HasTower;
    bool HasWall;

public:
    Position_field();

    void SetPlayer(bool value);
    void SetEnemy(bool value);
    void SetBlock(bool value);
    void SetTrap(bool value);
    void SetTower(bool value);

    bool isPlayer() const;
    bool isEnemy() const;
    bool isBlock() const;
    bool isTrap() const;
    bool isTower() const;

    bool free_pos() const;
    void clear_pos();
    char display_field() const;
};

#endif