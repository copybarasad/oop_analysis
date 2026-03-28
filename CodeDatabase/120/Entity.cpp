#include "Entity.h"

Entity::Entity(const Position& pos, int hp) : position(pos), health(hp) {}

void Entity::TakeDamage(int dmg) {
  health -= dmg;
  if (health < 0) health = 0;
}

bool Entity::IsAlive() const { return health > 0; }

Position Entity::GetPosition() const { return position; }

void Entity::SetPosition(const Position& pos) { position = pos; }

int Entity::GetHealth() const { return health; }

void Entity::SetHealth(int hp) { health = hp; }