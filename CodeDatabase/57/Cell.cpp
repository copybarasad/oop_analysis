#include <iostream>
#include "Cell.hpp"
#include "./Entities/Entity.hpp"


Cell::Cell() : status(ordinary), use(false), entity(nullptr) {}
Cell::Cell(std::string puth_png, Status speed) : status(speed),
 use(false), entity(nullptr) {
    texture.loadFromFile(puth_png);
    sprite.setTexture(texture);
}

bool Cell::Cell_get_use() const { return use; }
Entity* Cell::Cell_get_entity() const { return entity; }

void Cell::Cell_send_use(bool use, Entity* entity) {
    this->use = use;
    if (!use) {
        this->entity = nullptr;
    } else {
        this->entity = entity;
    }
}

sf::Sprite& Cell::getSprite() { return sprite; }

 