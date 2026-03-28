#include <Ceil.hpp>
#include <Floor.hpp>

Ceil::Ceil() : entity(nullptr) {}

void Ceil::setEntity(std::shared_ptr<Entity> newEntity) {
    entity = std::move(newEntity);
} 

std::shared_ptr<Entity> Ceil::getEntity() const { 
    return entity;
}

void Ceil::setDefault() {
}

void Ceil::draw() const {
    if (entity != nullptr) {
        entity->draw();
    }
}