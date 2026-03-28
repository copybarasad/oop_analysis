#include "Enemy.h"

Enemy::Enemy(bool IsPlayer, bool IsAlive, ATKtype type, std::string Body, 
             int HP, int ATK, int X_coord, int Y_coord, int Speed)
    :Humanoid(false, true, ATKtype::melee, "Ѫ ", HP, ATK, X_coord, Y_coord, Speed) 
{
    setSpeedCD(0);
    setATKCD(0);
}

Coordinates Enemy::moveto(Direction where) 
{
    switch(where) 
    {
        case Direction::KNOWHERE: return Coordinates(this->getX_coord(), this->getY_coord());
        case Direction::UP: return Coordinates(this->getX_coord(), this->getY_coord() - 1);
        case Direction::DOWN: return Coordinates(this->getX_coord(), this->getY_coord() + 1);
        case Direction::LEFT: return Coordinates(this->getX_coord() - 1, this->getY_coord());
        case Direction::RIGHT: return Coordinates(this->getX_coord() + 1, this->getY_coord());
        default: return Coordinates(this->getX_coord(),this->getY_coord());
    }
}

void Enemy::move(Coordinates destination, int damage, int Speed) 
{
    if (this->checkSpeedCD() == 0) 
    {   
        this->setXY_coord(destination.getX_coord(), destination.getY_coord());
        this->take_damage(damage);
        if (this->checkPulse() == false) return;
        this->setSpeed(Speed);
    } 
    else 
    {
        int howmuch;
        if (this->checkSpeedCD() < 0) { this->resetSpeedCD(0); howmuch = 0; }
        else if (this->checkSpeedCD() == 0) howmuch = 0;
        else howmuch = 1;
        setSpeedCD(checkSpeedCD() - howmuch);
    }
}

std::string Enemy::serialize() const 
{
    return Humanoid::serialize();
}

void Enemy::deserialize(const std::string& data) 
{
    Humanoid::deserialize(data);
}