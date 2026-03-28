#include "GameField.h"
#include "Character.h"
#include "Saloon.h"
#include "Constants.h"

using namespace GameConstants;

Saloon::Saloon(int x, int y) : position{x, y}, time_freeze(DEFAULT_FREEZE_TIME){}


int Saloon::getX() const { return position.x; }
int Saloon::getY() const { return position.y; }
int Saloon::getTime_freeze() const{ return time_freeze; }


void Saloon::setTime_freeze(int new_time) { 
    time_freeze = abs(new_time);
}

void Saloon::setPosition(int newX, int newY) { 
    position.x = newX; 
    position.y = newY; 
}