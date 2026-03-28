#ifndef TRAP_H
#define TRAP_H

class GameField;
class Enemy;

class Trap {
private:
    int damage;
    int duration;
    bool isActive;
    int x, y;

public:
    Trap(int trapDamage = 20, int trapDuration = 10);
    
    int getDamage() const;
    int getDuration() const;
    bool getIsActive() const;
    void getPosition(int& outX, int& outY) const;
    
    void place(int posX, int posY);
    bool checkTrigger(int enemyX, int enemyY, Enemy& enemy);
    void update();
    void remove();
};

#endif