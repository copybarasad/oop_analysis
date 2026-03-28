#ifndef ENEMYTOWER_H
#define ENEMYTOWER_H

class Player;
class GameController;

class EnemyTower {
public:
    EnemyTower(int x, int y, int health); 
    
    void processTurn(Player& player, GameController& game); 
    bool isAlive() const;
    void takeDamage(int amount);
    int getX() const;
    int getY() const;
    int getHealth() const;

private:
    int x;
    int y;
    int health;
    int cooldown;
    int turnsUntilAttack;
};

#endif