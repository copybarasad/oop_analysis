#pragma once
#include <iostream>


enum class GameEntType{
    Player,
    Enemy,
    Tower,

};

class GameEnt{

protected:
    double health;
    double damage;

    int pos_x;
    int pos_y;
        
    
    GameEntType type;


public:

    GameEnt(double healthObj, double damageObj, int pos_x, int pos_y, GameEntType typeObj);

    bool isAlive() const ;

    int getX() const {return pos_x;};

    GameEntType getObjType() const;

    virtual GameEnt* clone() const = 0;

    int getY() const {return pos_y;};

    double getMeleeDamage() const;

    int getHp() const {return health;};

    void setHealth(double newHealth) { 
        health = (newHealth < 0.0 ? 0.0 : newHealth); 
    }
    
    // Не константные методы

    void setPosition(int newX, int newY);

    void takeDamage(double gotedDamage); 


    



};