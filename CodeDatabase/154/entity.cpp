#include "entity.h"
#include "ally.h"

Entity::Entity(int id, int health): id_(id), health_(health) {}
Entity::~Entity() = default;
int Entity::getId() const { return id_; }
int Entity::getHealth() const { return health_; }
void Entity::setHealth(int health) { health_ = health; }
bool Entity::isAlive() const { return health_ > 0; }

Player* Entity::asPlayer() { return nullptr; }
Enemy* Entity::asEnemy() { return nullptr; }
Building* Entity::asBuilding() { return nullptr; }
Trap* Entity::asTrap() { return nullptr; }
Ally* Entity::asAlly() { return nullptr; }
Tower* Entity::asTower() { return nullptr; }
