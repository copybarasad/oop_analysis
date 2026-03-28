#include "ally.h"
#include "game.h"


Ally::Ally(int startX, int startY, int initialHealth, int initialDamage)
  : x(startX), y(startY), health(initialHealth), damage(initialDamage), slowed(false) {}

int Ally::getX() const {return x;}
int Ally::getY() const {return y;}
int Ally::getHealth() const {return health;}
int Ally::getDamage() const {return damage;}

void Ally::move(int newX, int newY) {
  x = newX;
  y = newY;
}

void Ally::takeDamage(int damage) {
  health -= damage;
  if (health<0) health=0;
}

void Ally::setSlowed(bool slowed) {
  this->slowed = slowed;
}

bool Ally::isAlive() const {return health>0;}

bool Ally::isSlowed() const {return slowed;}