#pragma once

class Entity{
protected:
    unsigned int x;
    unsigned int y;
    unsigned int lives;
public:
    Entity(){}
    virtual ~Entity(){}
    virtual unsigned int getX() const {
        return this->x;
    }
    virtual unsigned int getY() const{
        return this->y;
    }
    virtual void setPos(int x, int y){
        this->x = x;
        this->y = y;
    }
    virtual void setX(int x){
        this->x = x;
    }
    virtual void setY(int y){
        this->y = y;
    }
    virtual void takeDamage(unsigned int damage){
        lives = (lives > damage) ? (lives - damage) : 0;
    }

    virtual bool isAlive() const{
        return lives == 0 ? false : true;
    }
    virtual unsigned int getLives() const {
        return this->lives;
    }
};
