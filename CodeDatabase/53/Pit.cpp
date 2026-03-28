#include "GameField.h"
#include "Player.h"
#include "Pit.h"


Pit::Pit(int x, int y) : position{x, y}{}


int Pit::getX() const { return position.x; }
int Pit::getY() const { return position.y; }


void Pit::setPosition(int newX, int newY) { 
    position.x = newX; 
    position.y = newY; 
}

