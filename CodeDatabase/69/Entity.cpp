#include "Entity.h"

Entity::Entity(std::string id, Position pos, int hp) 
    : id_(std::move(id)), pos_(pos), hp_(hp) 
{}

Position Entity::GetPosition() const { 
    return pos_; 
}

void Entity::SetPosition(Position p) { 
    pos_ = p; 
}

std::string Entity::GetId() const { 
    return id_; 
}

int Entity::GetHp() const { 
    return hp_; 
}

bool Entity::IsAlive() const { 
    return hp_ > 0; 
}

void Entity::TakeDamage(int amount) {
    if (amount <= 0) return;
    hp_ -= amount;
    if (hp_ < 0) hp_ = 0;
}

void Entity::OnTurn(Game& game) {
    // 
}